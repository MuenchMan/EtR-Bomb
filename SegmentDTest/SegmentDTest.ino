
#include <Wire.h> // Include the Arduino SPI library

// Here we'll define the I2C address of our S7S. By default it should be 0x71. This can be changed, though.
const byte s7sTWOAddress = 0x71;
char testCode [4] = {'1', '2', '3', '4'};

void setup()
{
  Wire.begin();  // Initialize hardware I2C pins

  clearDisplayI2C(s7sTWOAddress);  // Clears display, resets cursor

  s7sSendStringI2C("-GO-",s7sTWOAddress); // start & reset signal
  setDecimalsI2C(0b111111, s7sTWOAddress);  // Turn on all decimals, colon, apos

  // Flash brightness values at the beginning
  setBrightnessI2C(0,s7sTWOAddress);  // Lowest brightness
  delay(1500);
  setBrightnessI2C(255, s7sTWOAddress);  // High brightness
  delay(1500);

  clearDisplayI2C(s7sTWOAddress); // Clear the display before jumping into loop
}

void loop()
{
  s7sSendStringI2C(testCode,s7sTWOAddress); // sends characters to the display

  delay(100);  // This will make the display update at 10Hz.*/
}  // -------- END OF VOID LOOP --------


void s7sSendStringI2C(String toSend, byte Address) // This custom function works somewhat like a serial.print - You can send it an array of chars (string) and it'll print - the first 4 characters in the array - change Address of target hardware
{
  Wire.beginTransmission(Address);
  for (int i = 0; i < 4; i++)
  {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}


void clearDisplayI2C(byte Address) // Send the clear display command (0x76) - This will clear the display and reset the cursor - change Address of target hardware
{
  Wire.beginTransmission(Address);
  Wire.write(0x76);  // Clear display command
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

// Turn on any, none, or all of the decimals - The six lowest bits in the decimals parameter sets a decimal (or colon, or apostrophe) on or off. 
// A 1 indicates on, 0 off - [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1) - change Address of target hardware
void setDecimalsI2C(byte decimals, byte Address)
{
  Wire.beginTransmission(Address);
  Wire.write(0x77);
  Wire.write(decimals);
  Wire.endTransmission();
}

