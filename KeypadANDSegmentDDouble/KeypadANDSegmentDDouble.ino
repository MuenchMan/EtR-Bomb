#include <Wire.h> // Include the Arduino SPI library
const byte s7sONEAddress = 0x72; // define the I2C address the S7S (default is 0x71)
const byte s7sTWOAddress = 0x73; // define the I2C address of our S7S (default is 0x71)

// ------ Keypad Magic
#include <Keypad.h>
const byte ROWSONE = 4; //four rows
const byte COLSONE = 3; //three columns
const byte ROWSTWO = 4; //four rows
const byte COLSTWO = 3; //three columns
char keysONE[ROWSONE][COLSONE] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
char keysTWO[ROWSTWO][COLSTWO] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPinsONE[ROWSONE] = {25, 24, 23, 22}; //connect to the row pinouts of the keypad
byte colPinsONE[COLSONE] = {28, 27, 26}; //connect to the column pinouts of the keypad
Keypad keypadONE = Keypad( makeKeymap(keysONE), rowPinsONE, colPinsONE, ROWSONE, COLSONE );

byte rowPinsTWO[ROWSTWO] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPinsTWO[COLSTWO] = {8, 7, 6}; //connect to the column pinouts of the keypad
Keypad keypadTWO = Keypad( makeKeymap(keysTWO), rowPinsTWO, colPinsTWO, ROWSTWO, COLSTWO );
// ------ Keypad Magic


// ------ Gameplay logic
char keypadVoid = '-'; //

char keypadONEEntry [4] = {keypadVoid, keypadVoid, keypadVoid, keypadVoid}; // creates keypad array with 4 slots
char keypadONECode [4] = {'1', '2', '3', '4'}; // code to be unlocked
bool keypadONEUnlock = false;

char keypadTWOEntry [4] = {keypadVoid, keypadVoid, keypadVoid, keypadVoid}; // creates keypad array with 4 slots
char keypadTWOCode [4] = {'1', '2', '3', '4'}; // code to be unlocked
bool keypadTWOUnlock = false;
// ------ Gameplay logic

void setup() {
  Serial.begin(9600);

  Wire.begin();  // Initialize hardware I2C pins
  s7sStartSequence(s7sONEAddress); // to communicate restart of system
  s7sStartSequence(s7sTWOAddress); // to communicate restart of system
}

void loop() {

  // ------ functions for keypadONE ------
  int keyONE = keypadONE.getKey(); // returns key if pressed
  s7sSendStringI2C(keypadONEEntry, s7sONEAddress); // displays keypadVoid if no entry happens
  if (keyONE) {
    // Serial.println(key);
    if (keypadONEEntry[0] == keypadVoid)  { // checks if the slot on the array is free and overwrites it with the keypress
      keypadONEEntry[0] = keyONE;
      s7sSendStringI2C(keypadONEEntry, s7sONEAddress);
    }
    else if (keypadONEEntry[1] == keypadVoid)   {
      keypadONEEntry[1] = keyONE;
      s7sSendStringI2C(keypadONEEntry, s7sONEAddress);
    }
    else if (keypadONEEntry[2] == keypadVoid)   {
      keypadONEEntry[2] = keyONE;
      s7sSendStringI2C(keypadONEEntry, s7sONEAddress);
    }
    else if (keypadONEEntry[3] == keypadVoid)   {
      keypadONEEntry[3] = keyONE;
      s7sSendStringI2C(keypadONEEntry, s7sONEAddress);
    }
  }

  if ((keypadONEEntry[0] != keypadVoid) &&  (keypadONEEntry[1] != keypadVoid) &&  (keypadONEEntry[2] != keypadVoid) &&  (keypadONEEntry[3] != keypadVoid)) { // checks if all slots are filled with keypresses
    if ((keypadONEEntry[0] == keypadONECode[0]) &&  (keypadONEEntry[1] == keypadONECode[1]) &&  (keypadONEEntry[2] == keypadONECode[2]) &&  (keypadONEEntry[3] == keypadONECode[3]))   { // checks if keypresses match the code
      keypadONEUnlock = true;
      Serial.println("unlockONETrue");
      setDecimalsI2C(0b00001111, s7sONEAddress);  // Sets digit 3 decimal on
    }
    else {
      keypadONEUnlock = false;
      Serial.println("unlockONEFalse");
      for (int i = 0; i < 4; i++)  { // overwrites all slots for re-entry
        keypadONEEntry [i] = keypadVoid;
      }
    }
  }

  // ------ functions for keypadTWO ------
  int keyTWO = keypadTWO.getKey(); // returns key if pressed
  s7sSendStringI2C(keypadTWOEntry, s7sTWOAddress); // displays keypadVoid if no entry happens
  if (keyTWO) {
    // Serial.println(key);
    if (keypadTWOEntry[0] == keypadVoid)  { // checks if the slot on the array is free and overwrites it with the keypress
      keypadTWOEntry[0] = keyTWO;
      s7sSendStringI2C(keypadTWOEntry, s7sTWOAddress);
    }
    else if (keypadTWOEntry[1] == keypadVoid)   {
      keypadTWOEntry[1] = keyTWO;
      s7sSendStringI2C(keypadTWOEntry, s7sTWOAddress);
    }
    else if (keypadTWOEntry[2] == keypadVoid)   {
      keypadTWOEntry[2] = keyTWO;
      s7sSendStringI2C(keypadONEEntry, s7sTWOAddress);
    }
    else if (keypadTWOEntry[3] == keypadVoid)   {
      keypadTWOEntry[3] = keyTWO;
      s7sSendStringI2C(keypadTWOEntry, s7sTWOAddress);
    }
  }

  if ((keypadTWOEntry[0] != keypadVoid) &&  (keypadTWOEntry[1] != keypadVoid) &&  (keypadTWOEntry[2] != keypadVoid) &&  (keypadTWOEntry[3] != keypadVoid)) { // checks if all slots are filled with keypresses
    if ((keypadTWOEntry[0] == keypadTWOCode[0]) &&  (keypadTWOEntry[1] == keypadTWOCode[1]) &&  (keypadTWOEntry[2] == keypadTWOCode[2]) &&  (keypadTWOEntry[3] == keypadTWOCode[3]))   { // checks if keypresses match the code
      keypadTWOUnlock = true;
      Serial.println("unlockTWOTrue");
      setDecimalsI2C(0b00001111, s7sTWOAddress);  // Sets digit 3 decimal on
    }
    else {
      keypadTWOUnlock = false;
      Serial.println("unlockTWOFalse");
      for (int i = 0; i < 4; i++)  { // overwrites all slots for re-entry
        keypadTWOEntry [i] = keypadVoid;
      }
    }
  }
}

void s7sSendStringI2C(String toSend, byte Address) // This custom function works somewhat like a serial.print - You can send it an array of chars (string) and it'll print - the first 4 characters in the array - change Address of target hardware
{
  Wire.beginTransmission(Address);
  for (int i = 0; i < 4; i++)
  {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}

// Set the displays brightness. Should receive byte with the value to set the brightness to
// dimmest------------->brightest 0--------127--------255 - change Address of target hardware
void setBrightnessI2C(byte value, byte Address)
{
  Wire.beginTransmission(Address);
  Wire.write(0x7A);  // Set brightness command byte
  Wire.write(value);  // brightness data byte
  Wire.endTransmission();
}

void s7sStartSequence (byte Address)
{
  clearDisplayI2C(Address);  // Clears display, resets cursor

  s7sSendStringI2C("-GO-", Address); // start & reset signal
  // Flash brightness values at the beginning
  setBrightnessI2C(0, Address); // Lowest brightness
  delay(1500);
  setBrightnessI2C(255, Address);  // High brightness
  delay(1500);

  clearDisplayI2C(Address); // Clear the display before jumping into loop
}

void clearDisplayI2C(byte Address) // Send the clear display command (0x76) - This will clear the display and reset the cursor - change Address of target hardware
{
  Wire.beginTransmission(Address);
  Wire.write(0x76);  // Clear display command
  Wire.endTransmission();
}

// Turn on any, none, or all of the decimals - The six lowest bits in the decimals parameter sets a decimal (or colon, or apostrophe) on or off.
// A 1 indicates on, 0 off - [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1) - change Address of target hardware
void setDecimalsI2C(byte decimals, byte Address)
{
  Wire.beginTransmission(Address);
  Wire.write(0x77);
  Wire.write(decimals);
  Wire.endTransmission();
}


