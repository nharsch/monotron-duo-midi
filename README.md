# Monotron Duo Midi Support
Adding MIDI control to the Korg Monotron Duo with Arduino

## Intro
The Korg Monotron Duo is a small ribbon controller pocket synth I got as a stocking stuffer a couple years ago. It's pretty cool for what it is, but playing the tiny ribbon controller gets old pretty quick. I wanted to see if I could sequence it, but there are no built in CV or MIDI inputs.

_But_, Korg kinly included some big, solder friendly patch points:
![](https://hackaday.com/wp-content/uploads/2020/09/korg-monotron-pcb.png)

Korg also published a [public schematic](https://www.korg.com/download/global/monotron_duo_schematic/monotron_DUO_sch.pdf)

## Controlling `Vrib` with DAC output
I decided the most promising route would be to use the `VRib` patch point. This patch point is placed after the ribbon keyboard, which is basically a big variable resistor, which shifts the voltage at `VRib` between 0 - 3.3V. 
The 0-3.3v octave range maps to a 18 steps (~ 1 1/2 octave). Luckily, the operational voltage of the Duo and my Teensyduino is about the same, so I can power the Duo with the Teensyduino and control the voltage range on `Vrib` with analog pin output.

### PWM vs DAC
My Teensy has several pins capable of sending PWM output, and one pin capable of sending DAC analog output. PWM works by rapidly switiching between high and low, whereas the DAC output sends a steady voltage. 
PWM _can_ be low pass filtered to generate a steady voltage, but voltage changes are slow, and we want to play music, so DAC output it is.

### No need for `Gate`
Usually, with CV controlled synths, a pitch CV signal and a GATE signal is needed. Pitch CV changes the pitch, and GATE is a note on/off control. I thought I would need to send digital on/off signal to the `Gate` patch point, but it turns off that raising the voltage on `Vrib` automatically sounds a note on the Duo.

## MIDI to CV (sorta)
I used the Arduino `usbMIDI` library to receive midi messages over the USB port. MIDI represents note values with a 7 bit (actually a byte, but the highest bit is always 0), giving us 128 note values.
There are existing MIDI to CV libraries, but the `Vrib` voltage is not CV spec, which is one volt per octave, but a 3.3v to ~1.5 range. Each note on the Vrib represents a ~0.18v change.

I wrapped the incoming midi notes around one octave, so playing C0, C1, C2 etc sends the same voltage to `Vrib`

## Multiple notes?
The Duo is monophonic, it can only play one note at a time. Sounds easy enough to implement. However, a player will often hold down several notes on a keyboard, even when playing a monophonic instrument, in order to "pull off" notes rapidly. Each MIDI note message will only send the most recent NOTE ON or NOTE OFF message. The teensyduino needs to remember notes that are being held (a NOTE ON message was sent, but no NOTE OFF message sent).

I built a `NoteStack` in C, which ended up being a quick Data Structures and Algorithm refresher, and a lesson in C memory management. 

## It works!
I'm now able to sequence the Duo with Ableton, which frees up my hands to play with the knobs on the Duo, which is the really fun part of analog synths.

## What next?
### More octaves and Pitch Bend
One octave is cool, but the Duo has about a 3 octave range if you adjust the pitch knob. Bizarely, there is no equivalent pitch patch point. Even if there was, I'm already out of DAC outputs. I experimented using PWM on the output pin of the pitch potentiometer and it _sorta_ worked but wasn't great. 
I may try replacing the pitch pot with a digital potentiometer. I could then adjust the octave with macro adjustments to the pot, and implement pitch bending with smaller adjustments. What I'd lose is the ability to tune the device by hand, and tuning with MIDS Sysex messages seems fidly. I could maybe use a rotary controller to send simple pitch up/down messages to offset the pot values...

### MIDI DIN input
MIDI over USB is great, and increasingly standard, but I have an old school MIDI Din out on my interface that I'm already using with another analog synth. I'd like to daisy chain the duo to that midi line. 
MIDI is a UART protocol, and Arduino has libraries to handle MIDI over serial. 
Right now, the Duo listens to all Midi channels, I'd need to assign it a midi channel, so that my one MIDI bus can control multiple synths independently.

### Packaging
The Duo's enclosure doesn't have much free space, but I'm pretty sure I can squeeze the teensy in there. I'll need to route a hole for the USB input _or_ for a MIDI din input. I could maybe use the existing Aux input 3.5mm jack to use with a 3.5mm - MIDI DIN breakout cable. 
