uint8_t RIB_PIN = A14;
uint8_t CV_PIN = A9;
uint8_t GATE_PIN = 2;
bool GATE = false;
uint8_t CV = 0;
uint8_t note_ = 0; // 16 semitones on ribbon
uint8_t LOWEST_NOTE = 9;
uint16_t interval_ = 255 / 16;


void SOnNoteOn(byte channel, byte note, byte velocity)
{
//    gMidiGateOn = true;
    Serial.println();
    Serial.print(note);
    Serial.println();
    note_ = note % 12;
    GATE = true;
    Serial.print(note_);
    Serial.println();
}

void SOnNoteOff(byte channel, byte note, byte velocity)
{
    GATE = false;
}

void setup() {
  // put your setup code here, to run once:
    //Initiate Serial communication.
  usbMIDI.setHandleNoteOn(SOnNoteOn); 
  usbMIDI.setHandleNoteOff(SOnNoteOff); 
  Serial.begin(9600);
  Serial.write("begin");
  pinMode(RIB_PIN, OUTPUT);
  pinMode(CV_PIN, OUTPUT);
  pinMode(GATE_PIN, OUTPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  usbMIDI.read();
  digitalWrite(GATE_PIN, GATE);
  if (GATE) {
      analogWrite(RIB_PIN, (note_ * interval_) + interval_);
  } else {
    analogWrite(RIB_PIN, 0);
  }
}
