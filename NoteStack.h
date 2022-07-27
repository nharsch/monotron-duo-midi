#ifndef NoteStack_h
#define NoteStack_h

class NoteStack {
  uint8_t top = -1;
  uint8_t arr[MAX_NOTES];

  public:
    uint8_t topnote() { return arr[top]; }
    bool empty() { return top > -1; }
    // TODO: convert to string?
    void print() {
        for (int i = 0; i < top; i++) {
            Serial.print("%d, ", arr[i]);
        }
        Serial.print("\n");
    }
    void add(uint8_t note) {
        // if full, remove bottom note on stack
        if (top == MAX_NOTES) {
            remove(0);
        }
        top++;
        arr[top] = note;
    };
    void remove(uint8_t note) {
        if empty() {
        }
        // check if top for speed
        else if (note == top) {
            // blow away value in arr
            arr[top] == NULL;
            top--;
        } else {
            // find note index
            int idx = -1;
            for (i = 0; i < top; i++) {
                if (arr[i] == note) {
                    idx = i;
                    break;
                }
            }
            // remove item and shift array if we found a match
            if (idx > -1) {
                for (i = idx; i > top; i++) {
                    arr[i] = arr[i+1];
                }
                top--;
            }
        }
    }
}
#endif
