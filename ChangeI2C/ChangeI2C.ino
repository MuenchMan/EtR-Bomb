
#include <Wire.h> // Include the Arduino SPI library

void setup()
{
  Wire.begin();  // Initialize hardware I2C pins

}

void loop()
{
  Wire.beginTransmission(0x71); // enter current address
  Wire.write(0x80); // code for changing address
  Wire.write(0x72); // new address
  Wire.endTransmission();
}  // -------- END OF VOID LOOP --------

