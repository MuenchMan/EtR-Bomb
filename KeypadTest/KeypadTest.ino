// ----- Keypad Magic
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {25, 24, 23, 22}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {28, 27, 26}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// ----- Keypad Magic

char keypadVoid = '-';
char keypadOneEntry [4] = {keypadVoid, keypadVoid, keypadVoid, keypadVoid}; // creates keypad array with 4 slots
char keypadOneCode [4] = {'1', '2', '3', '4'};
bool keypadOneUnlock = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int key = keypad.getKey();

  if (key) {
    // Serial.println(key);
    if (keypadOneEntry[0] == keypadVoid)  { // checks if the slot on the array is free and overwrites it with the keypress
      keypadOneEntry[0] = key;
    }
    else if (keypadOneEntry[1] == keypadVoid)   {
      keypadOneEntry[1] = key;
    }
    else if (keypadOneEntry[2] == keypadVoid)   {
      keypadOneEntry[2] = key;
    }
    else if (keypadOneEntry[3] == keypadVoid)   {
      keypadOneEntry[3] = key;
    }
  }

  if ((keypadOneEntry[0] != keypadVoid) &&  (keypadOneEntry[1] != keypadVoid) &&  (keypadOneEntry[2] != keypadVoid) &&  (keypadOneEntry[3] != keypadVoid)) { // checks if all slots are filled with keypresses
    if ((keypadOneEntry[0] == keypadOneCode[0]) &&  (keypadOneEntry[1] == keypadOneCode[1]) &&  (keypadOneEntry[2] == keypadOneCode[2]) &&  (keypadOneEntry[3] == keypadOneCode[3]))   { // checks if keypresses match the code
      keypadOneUnlock = true;
      Serial.println("unlockTrue");
    }
    else {
      keypadOneUnlock = false;
      Serial.println("unlockFalse");
      for (int i = 0; i < 4; i++)  { // overwrites all slots for re-entry
        keypadOneEntry [i] = keypadVoid;
      }
    }
  }
}
