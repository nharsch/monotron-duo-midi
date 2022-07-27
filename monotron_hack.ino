#define MAX_NOTES 12

const uint8_t RIB_PIN = A14;
const uint8_t CV_PIN = A9;
const uint8_t GATE_PIN = 2;
const uint8_t LOWEST_NOTE = 9;
const uint16_t interval_ = 255 / 16;  // TODO: use 12 bit DAC values

bool GATE = false;
uint8_t CV = 0;


class NoteStack {
  private:
    int top = -1;
    uint8_t arr[MAX_NOTES];
    void remove(int idx) {
      Serial.print("calling remove\n");
      if (idx > -1) {
          for (int i = idx; i < this->top; i++) {
              this->arr[i] = this->arr[i+1];
          }
          this->top--;
      }
    };
    
  public:
    NoteStack() {};
    bool empty() { return this->top < 0; }
    uint8_t topnote() { 
      if (this->top > -1) {
        return this->arr[this->top]; 
      } 
    }
    // TODO: convert to string?
    void print() {
        Serial.print("NoteStack - top: ");
        Serial.print(this->top);
        Serial.print(" notes: ");
        for (int i = 0; i <= this->top; i++) {
            Serial.print(this->arr[i]);
            Serial.print(" ");
        }
    }
    void addNote(uint8_t note) {
        // if full, remove bottom note on stack
        if (this->top >= MAX_NOTES) {
            this->remove(0);
        }
        this->top++;
        this->arr[this->top] = note;
    };
    void removeNote(uint8_t note) {
        Serial.print("remove note called\n");
        if (this->top < 0) {
//            Serial.print("remove called on empty\n");
        }
        // check if top for speed
        else if (note == this->arr[this->top]) {
//            Serial.print("remove called on top note match\n");
            this->top--;
        } else {
//            Serial.print("remove called on middle search match\n");
            // find note index
            int idx = -1;
            for (int i = 0; i < this->top; i++) {
                if (this->arr[i] == note) {
                    idx = i;
                    break;
                }
            }
            this->remove(idx);
        }
    }
};

NoteStack notes = NoteStack();

void SOnNoteOn(byte channel, byte note, byte velocity)
{
    uint8_t wrpdnote = note % 12;
//    Serial.print("wrapped note: ");
//    Serial.print(wrpdnote);
//    Serial.println();
    notes.addNote(wrpdnote);
//    notes.print();
}

void SOnNoteOff(byte channel, byte note, byte velocity)
{
    uint8_t wrpdnote = note % 12;
//    Serial.print("wrapped note: ");
//    Serial.print(wrpdnote);
//    Serial.println();
//    Serial.print("note off\n");
    notes.removeNote(wrpdnote);
//    notes.print();
}

void setup() {
  usbMIDI.setHandleNoteOn(SOnNoteOn); 
  usbMIDI.setHandleNoteOff(SOnNoteOff); 
  //Initiate Serial communication.
  Serial.begin(9600);
  Serial.write("begin");
  pinMode(RIB_PIN, OUTPUT);
  pinMode(CV_PIN, OUTPUT);
  pinMode(GATE_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  usbMIDI.read();
  if (!notes.empty()) {
      uint8_t cnote = notes.topnote();
      analogWrite(RIB_PIN, (cnote * interval_) + interval_);
  } else {
      analogWrite(RIB_PIN, 0);
  }
}
