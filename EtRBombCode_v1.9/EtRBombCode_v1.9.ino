// ------ I2C-Communication FOR SEGMENT DISPLAYS
#include <Wire.h> // Include the Arduino SPI library
const byte s7sONEAddress = 0x72; // define the I2C address the S7S (default is 0x71)
const byte s7sTWOAddress = 0x74; // define the I2C address of the S7S (default is 0x71)

// ------ SERIAL Communication for Main Display
#include <SoftwareSerial.h>
const int softwareTx = 43;
const int softwareRx = 7;

SoftwareSerial s7s(softwareRx, softwareTx);

unsigned int counter = 0;  // This variable will count up to 65k
char tempString[10];  // Will be used with sprintf to create strings

// ------ KEYPAD MAGIC
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
byte rowPinsONE[ROWSONE] = {28, 26, 24, 22}; //connect to the row pinouts of the keypad
byte colPinsONE[COLSONE] = {34, 32, 30}; //connect to the column pinouts of the keypad
Keypad keypadONE = Keypad( makeKeymap(keysONE), rowPinsONE, colPinsONE, ROWSONE, COLSONE );
byte rowPinsTWO[ROWSTWO] = {44, 42, 40, 38}; //connect to the row pinouts of the keypad
byte colPinsTWO[COLSTWO] = {50, 48, 46}; //connect to the column pinouts of the keypad
Keypad keypadTWO = Keypad( makeKeymap(keysTWO), rowPinsTWO, colPinsTWO, ROWSTWO, COLSTWO );

// ------ LEDs
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN 8
#define NUMPIXELS 12
Adafruit_NeoPixel strips = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int redPixelFill = 1;

int ledPin1 = 2; // LED on the very left of the fusebox
int ledPin2 = 3;
int ledPin3 = 4;
int ledPin4 = 5;
int ledPin5 = 6;
int ledPin6 = 7;

bool led1 = true;
bool led2 = true;
bool led3 = true;
bool led4 = true;
bool led5 = true;
bool led6 = true;
bool led7 = true; // ???

int inPin1 = 23;   // Wire on the very left of the fusebox
int inPin2 = 25;
int inPin3 = 27;
int inPin4 = 29;
int inPin5 = 31;
int inPin6 = 33;

int in1Val = 0; // val to read the Input of the according wire
int in2Val = 0;
int in3Val = 0;
int in4Val = 0;
int in5Val = 0;
int in6Val = 0;

bool in1 = true; // true == cable connected / false == cable disconnected
bool in2 = true;
bool in3 = true;
bool in4 = true;
bool in5 = true;
bool in6 = true;

int Wire1 = 0; // 0 is for wrong wire and 1 is for the correct wire
int Wire2 = 1;
int Wire3 = 0;
int Wire4 = 0;
int Wire5 = 0;
int Wire6 = 0;


// ------ Gameplay logic
char keypadVoid = '-'; // nulls the arrayitems and displays a "-"
char keypadONEEntry [4] = {keypadVoid, keypadVoid, keypadVoid, keypadVoid}; // creates keypad array with 4 slots
char keypadONECode [4] = {'6', '2', '4', '8'}; // code to be unlocked QUANTUMLOCK
bool keypadONEUnlock = false;
char keypadTWOEntry [4] = {keypadVoid, keypadVoid, keypadVoid, keypadVoid}; // creates keypad array with 4 slots
char keypadTWOCode [4] = {'3', '7', '2', '5'}; // code to be unlocked CRYPTOLOCK
bool keypadTWOUnlock = false;

int safeLockPin = 52; //SafeLock Pin
int blinkLED = 13; //SafeLock LED
bool safeLockTriggered = false;
bool safeLockLED = true;

// ------ timer 
int startMinutes = 15;
int minutes = startMinutes;
int seconds = 30;
String time;
int minuteCounter = 1;

unsigned long pMillisCountdown = 0;
unsigned long pMillisStrips = 0;
unsigned long pMillisBeep = 0;
unsigned long beepDelay = 0;
unsigned long cMillis = 0;
unsigned long oneSecond = 1000;

int piezoONEPin = 12; // pin connected to the Piezos

int opening = 0; // in use?
int armed = 0; // in use?
// 1=countdown 2=won 3=gameover
int state = 1;

bool keyPadONETrigger = false;
bool keyPadTWOTrigger = false;

int countdownFX = 0;
int currentPixel = 0;

void setup() { // SETUP BEGINS --- SETUP BEGINS --- SETUP BEGINS

  // SERIAL MAIN-Segment-Display
  s7s.begin(9600); // sets baud-rate - serial Main Segment-Display
  clearDisplay();  // Clears display, resets cursor - serial Main Segment-Display
  s7s.print("-GO-");
   setBrightness(0);  // Lowest brightness
  delay(1500);
  setBrightness(127);  // Medium brightness
  delay(1500);
  setBrightness(255);  // High brightness
  delay(1500);
  clearDisplay(); 
  
  Serial.begin(9600);
  Wire.begin(); // Initialize hardware I2C pins

  s7sStartSequence(s7sONEAddress); // to communicate restart of system
  s7sStartSequence(s7sTWOAddress); // to communicate restart of system

  //pinMode(bangPin, OUTPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);  // declare LED as output
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);

  pinMode(inPin1, INPUT_PULLUP);    // declare Wire as pushbutton with internal pullup resistors
  pinMode(inPin2, INPUT_PULLUP);
  pinMode(inPin3, INPUT_PULLUP);
  pinMode(inPin4, INPUT_PULLUP);
  pinMode(inPin5, INPUT_PULLUP);
  pinMode(inPin6, INPUT_PULLUP);

  pinMode(safeLockPin, INPUT_PULLUP); //SafeLock Pin
  pinMode(13, OUTPUT); //SafeLock LED
 
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  // --- NEOPIXEL-MAGIC ---
  strips.begin();
  strips.show(); // Initialize all pixels to 'off'
  colorWipe(strips.Color(0, 0, 255), 50); // Blue
  
  // EXPERIMENTAL DELAY --- TEST --- TEST --- TEST
  //delay(80000);

} // --- END OF SETUP ---



void loop() { // LOOP BEGINS --- LOOP BEGINS --- LOOP BEGINS

  // ------ FUSEBOX-CABLES ------
  in1Val = digitalRead(inPin1);  // read input value of Wire1
  if (in1Val == LOW);   {
    in1 = true;
  }
  if (in1Val == HIGH) {
    in1 = false;
  }
  in2Val = digitalRead(inPin2);  // read input value of Wire2
  if (in2Val == LOW);   {
    in2 = true;
  }
  if (in2Val == HIGH) {
    in2 = false;
  }
  in3Val = digitalRead(inPin3);  // read input value of Wire3
  if (in3Val == LOW);   {
    in3 = true;
  }
  if (in3Val == HIGH) {
    in3 = false;
  }
  in4Val = digitalRead(inPin4);  // read input value of Wire4
  if (in4Val == LOW);   {
    in4 = true;
  }
  if (in4Val == HIGH) {
    in4 = false;
  }
  in5Val = digitalRead(inPin5);  // read input value of Wire5
  if (in5Val == LOW);   {
    in5 = true;
  }
  if (in5Val == HIGH) {
    in5 = false;
  }
  in6Val = digitalRead(inPin6);  // read input value of Wire6
  if (in6Val == LOW);   {
    in6 = true;
  }
  if (in6Val == HIGH) {
    in6 = false;
  }

  if ((Wire1 == 0) && (in1 == false)) { // Checks if wrong Wire is cut (turn into for-loop?) // unnecessary?
    state = 3;
  }
  if ((Wire2 == 0) && (in2 == false)) {
    state = 3;
  }
  if ((Wire3 == 0) && (in3 == false)) {
    state = 3;
  }
  if ((Wire4 == 0) && (in4 == false)) {
    state = 3;
  }
  if ((Wire5 == 0) && (in5 == false)) {
    state = 3;
  }
  if ((Wire6 == 0) && (in6 == false)) {
    state = 3;
  }

  if ((Wire1 == 1) && (in1 == false)) { // Checks if correct Wire is cut (turn into for-loop?)
    state = 2;
  }
  if ((Wire2 == 1) && (in2 == false)) {
    state = 2;
  }
  if ((Wire3 == 1) && (in3 == false)) {
    state = 2;
  }
  if ((Wire4 == 1) && (in4 == false)) {
    state = 2;
  }
  if ((Wire5 == 1) && (in5 == false)) {
    state = 2;
  }
  if ((Wire6 == 1) && (in6 == false)) {
    state = 2;
  }


  // ------ FUSEBOX-LEDS ------
  if (keypadONEUnlock == true)   { // turns leds for wrong cables off WARNING - make sure to select the correct leds!
    led1 = false;
    led6 = false;
  }
  if (keypadTWOUnlock == true)   {
    led3 = false;
    led4 = false;
    led5 = false;
  }


  if (led1 == true)   { // --- led1 - ON/OFF
    digitalWrite(ledPin1, HIGH);  // turn LED ON
  }
  else {
    digitalWrite(ledPin1, LOW);  // turn LED ON
  }
  if (led2 == true)   { // --- led2 - ON/OFF
    digitalWrite(ledPin2, HIGH);  // turn LED ON
  }
  else {
    digitalWrite(ledPin2, LOW);  // turn LED ON
  }
  if (led3 == true)   { // --- led3 - ON/OFF
    digitalWrite(ledPin3, HIGH);  // turn LED ON
  }
  else {
    digitalWrite(ledPin3, LOW);  // turn LED ON
  }
  if (led4 == true)   { // --- led4 - ON/OFF
    digitalWrite(ledPin4, HIGH);  // turn LED ON
  }
  else {
    digitalWrite(ledPin4, LOW);  // turn LED ON
  }
  if (led5 == true)   { // --- led5 - ON/OFF
    digitalWrite(ledPin5, HIGH);  // turn LED ON
  }
  else {
    digitalWrite(ledPin5, LOW);  // turn LED ON
  }
  if (led6 == true)   { // --- led6 - ON/OFF
    digitalWrite(ledPin6, HIGH);  // turn LED ON
  }
  else {
    digitalWrite(ledPin6, LOW);  // turn LED ON
  }
// ------ DEBUG ------
// Serial.print(strips.numPixels());
Serial.print(" state: ");
Serial.println(state);
//Serial.print(" keypadONEEntry ");
//Serial.print(keypadONEEntry);
Serial.print( " minutes:");
Serial.print(minutes);
Serial.print( " seconds:");
Serial.print(seconds);
  
  int safeLock = digitalRead(52);
  cMillis = millis();
     
  beepOFF(); // kills the piezo if millis are ellapsed
  
    if (safeLock == HIGH && safeLockTriggered == false)  { // HIGH - circuit is open caused by pullup resistor
      safeLockTriggered = true;
      safeLockLED = false;
      
      colorBlinkRed(100,16); // Red Blink
      colorUpdate(strips.Color(255, 0, 0), 0,redPixelFill); // Red
    if   (minutes>=2)  {
      minutes--;
      minutes--;
    }
    else  {
    minutes = 0;
    }
  }
  
  if (safeLockLED == true)  {
    digitalWrite(blinkLED, HIGH);
  }
  else  {
    digitalWrite(blinkLED, LOW);
  }
  
    if (state == 1) {// ------ COUNTDOWN-LOOP-START ------
    
    if (minutes >= 10 && seconds < 10) {
      time = String(minutes) + "0" + String(seconds);
    }
    if (minutes >= 10 && seconds >= 10) {
      time = String(minutes) + String(seconds);
    }
    if (minutes < 10 && seconds < 10) {
      time = "0" + String(minutes) + "0" + String(seconds);
    }
    if (minutes < 10 && seconds >= 10) {
      time = "0" + String(minutes) + String(seconds);
    }
    s7s.print(time); // displays string to main display
    Serial.print("CHECK");   

    if (millis() - pMillisCountdown > oneSecond) {
      pMillisCountdown = millis();

      if (seconds <= 0) {
        //Serial.print("-minute");
        clearDisplay();
        setDecimals(0b00010000);
        minutes--;
        seconds = 60;
        if (minuteCounter == 2)  {
          redPixelFill++;
          minuteCounter = 1;
        }
        else  {
        minuteCounter = 2;
        }
      }
      
      //Serial.print(time);
      //Serial.print(" time - ");  
      beepON(200);
      
      seconds--;
      countdownFX = 1;
      
      if (minutes <= 0 && seconds <= 1) {  
      state = 3;
      //Serial.print("FAILOVER"); 
  }
    }
  } // ------ COUNTDOWN-LOOP-END ------
  
  // ------ GAMEPLAY-WON ------
  if (state == 2) {
    beepOFF(); // kills the piezo if millis are ellapsed
    countdownFX = 0;
    
    for (int i = 0 ; i < 40 ; i++) {
      delay(50);
    }
    s7s.print("0000");
    colorUpdate(strips.Color(0, 0, 255), 200, strips.numPixels()); // Blue
    clearDisplay();
    s7s.print("SAFE");
    delay(60000);
  }

  // ------ GAMEPLAY-GAMEOVER ------
  if (state == 3) { 
    /// BANG!
    pinMode(10, OUTPUT);
    
    beepOFF(); // kills the piezo if millis are ellapsed
    countdownFX = 0;
    
    s7s.print("0000");
    colorWipe(strips.Color(255, 0, 0), 200);
    colorUpdate(strips.Color(255, 0, 0), 0, strips.numPixels()); // Red
    colorBlinkRed(500,6); // Red Blink
    clearDisplay();
    s7s.print("FAIL");
    beep(500);
    delay(10000000); // why the massive delay??? BECAUSE MOTHERFUCKER!!!
  }
  
// ------ COUNTDOWN-FX ------
  if(countdownFX == 1)  {
    if (millis() - pMillisStrips > 40)  {
      pMillisStrips = millis();
   
      strips.setPixelColor(currentPixel,strips.Color(255, 0, 0));
      strips.show();
      currentPixel++;
      
      if(currentPixel == NUMPIXELS){
        colorUpdate(strips.Color(255, 0, 0), 0, redPixelFill);
        currentPixel = 0;
        countdownFX = 0;
      }
    }
  }
  
    // ------ code for keypadONE ------
  int keyONE = keypadONE.getKey(); // returns key if pressed
  s7sSendStringI2C(keypadONEEntry, s7sONEAddress); // displays keypadVoid if no entry happens
  if (keyONE) {
    if (keypadONEEntry[0] == keypadVoid) { // checks if the slot on the array is free and overwrites it with the keypress
      keypadONEEntry[0] = keyONE;
      s7sSendStringI2C(keypadONEEntry, s7sONEAddress);
    }
    else if (keypadONEEntry[1] == keypadVoid) {
      keypadONEEntry[1] = keyONE;
      s7sSendStringI2C(keypadONEEntry, s7sONEAddress);
    }
    else if (keypadONEEntry[2] == keypadVoid) {
      keypadONEEntry[2] = keyONE;
      s7sSendStringI2C(keypadONEEntry, s7sONEAddress);
    }
    else if (keypadONEEntry[3] == keypadVoid) {
      keypadONEEntry[3] = keyONE;
      s7sSendStringI2C(keypadONEEntry, s7sONEAddress);
    }
  }
  if ((keypadONEEntry[0] != keypadVoid) && (keypadONEEntry[1] != keypadVoid) && (keypadONEEntry[2] != keypadVoid) && (keypadONEEntry[3] != keypadVoid)) { // checks if all slots are filled with keypresses
    if ((keypadONEEntry[0] == keypadONECode[0] || keypadONEEntry[0] == 'S') && (keypadONEEntry[1] == keypadONECode[1] || keypadONEEntry[1] == 'A') && (keypadONEEntry[2] == keypadONECode[2] || keypadONEEntry[2] == 'F') && (keypadONEEntry[3] == keypadONECode[3] || keypadONEEntry[3] == 'E')) { // checks if keypresses match the code
      keypadONEUnlock = true;
      //Serial.println("unlockONETrue");
      setDecimalsI2C(0b00001111, s7sONEAddress); // Sets digit 3 decimal on
      keypadONEEntry[0] = 'S';
      keypadONEEntry[1] = 'A';
      keypadONEEntry[2] = 'F';
      keypadONEEntry[3] = 'E';
    }
    else {
      keypadONEUnlock = false;
      //Serial.println("unlockONEFalse");
      colorBlinkRed(100,3); // Red Blink
      colorUpdate(strips.Color(255, 0, 0), 0,redPixelFill); // Red

      if(minutes>=1)  {
        minutes--;
      }
      else  {
        minutes = 0;
        seconds = 1;
      }
      
      for (int i = 0; i < 4; i++) { // overwrites all slots for re-entry
        keypadONEEntry [i] = keypadVoid;
      }
    }
  }
  // ------ code for keypadTWO ------
  int keyTWO = keypadTWO.getKey(); // returns key if pressed
  s7sSendStringI2C(keypadTWOEntry, s7sTWOAddress); // displays keypadVoid if no entry happens
  if (keyTWO) {
    if (keypadTWOEntry[0] == keypadVoid) { // checks if the slot on the array is free and overwrites it with the keypress
      keypadTWOEntry[0] = keyTWO;
      s7sSendStringI2C(keypadTWOEntry, s7sTWOAddress);
    }
    else if (keypadTWOEntry[1] == keypadVoid) {
      keypadTWOEntry[1] = keyTWO;
      s7sSendStringI2C(keypadTWOEntry, s7sTWOAddress);
    }
    else if (keypadTWOEntry[2] == keypadVoid) {
      keypadTWOEntry[2] = keyTWO;
      s7sSendStringI2C(keypadTWOEntry, s7sTWOAddress);
    }
    else if (keypadTWOEntry[3] == keypadVoid) {
      keypadTWOEntry[3] = keyTWO;
      s7sSendStringI2C(keypadTWOEntry, s7sTWOAddress);
    }
  }
  if ((keypadTWOEntry[0] != keypadVoid) && (keypadTWOEntry[1] != keypadVoid) && (keypadTWOEntry[2] != keypadVoid) && (keypadTWOEntry[3] != keypadVoid)) { // checks if all slots are filled with keypresses
    if ((keypadTWOEntry[0] == keypadTWOCode[0] || keypadTWOEntry[0] == 'S') && (keypadTWOEntry[1] == keypadTWOCode[1] || keypadTWOEntry[1] == 'A') && (keypadTWOEntry[2] == keypadTWOCode[2] || keypadTWOEntry[2] == 'F') && (keypadTWOEntry[3] == keypadTWOCode[3] || keypadTWOEntry[3] == 'E')) { // checks if keypresses match the code
      keypadTWOUnlock = true;
      //Serial.println("unlockTWOTrue");
      setDecimalsI2C(0b00001111, s7sTWOAddress); // Sets digit 3 decimal on
      keypadTWOEntry[0] = 'S';
      keypadTWOEntry[1] = 'A';
      keypadTWOEntry[2] = 'F';
      keypadTWOEntry[3] = 'E';

    }
    else {
      keypadTWOUnlock = false;
      //Serial.println("unlockTWOFalse");
      colorBlinkRed(100,3); // Red Blink
      colorUpdate(strips.Color(255, 0, 0), 0,redPixelFill); // Red
      
      if(minutes>=1)  {
        minutes--;
      }
      else  {
        minutes = 0;
        seconds = 1;
      }
      
      for (int i = 0; i < 4; i++) { // overwrites all slots for re-entry
        keypadTWOEntry [i] = keypadVoid;
      }
    }
  }
  
    // GREENLIGHT
  if (keyPadONETrigger == false && keypadONEUnlock == true)  {
     colorBlinkGreen(100,3);
     keyPadONETrigger = true;
  }
  if (keyPadTWOTrigger == false && keypadTWOUnlock == true)  {
     colorBlinkGreen(100,3);
     keyPadTWOTrigger = true;
  }
} // LOOP-END





// ------ FUNCTIONS ------
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
  Wire.write(0x7A); // Set brightness command byte
  Wire.write(value); // brightness data byte
  Wire.endTransmission();
}
void s7sStartSequence (byte Address)
{
  clearDisplayI2C(Address); // Clears display, resets cursor
  s7sSendStringI2C("-GO-", Address); // start & reset signal
  // Flash brightness values at the beginning
  setBrightnessI2C(0, Address); // Lowest brightness
  delay(500);
  setBrightnessI2C(255, Address); // High brightness
  delay(500);
  clearDisplayI2C(Address); // Clear the display
}
void clearDisplayI2C(byte Address) // Send the clear display command (0x76) - This will clear the display and reset the cursor - change Address of target hardware
{
  Wire.beginTransmission(Address);
  Wire.write(0x76); // Clear display command
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

void beep(int delayMS) {   
  analogWrite(piezoONEPin, 250); // Almost any value can be used except 0 and 255 experiment to get the best tone
   delay(delayMS);
  analogWrite(piezoONEPin, 0); // 0 turns it off
}
void beepON(const long delayMS) {   
  pMillisBeep = millis();
  analogWrite(piezoONEPin, 250); // Almost any value can be used except 0 and 255 experiment to get the best tone
   beepDelay = delayMS; 
}
void beepOFF() {    
  if(millis() - pMillisBeep > beepDelay)  {
    analogWrite(piezoONEPin, 0); // 0 turns it off
  } 
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strips.numPixels(); i++) {
    strips.setPixelColor(i, c);
    strips.show();
    delay(wait);
  }
}

void colorBlinkRed(int delayMS, int times) {
  for (int i = 0; i< times; i++)  {
    
        beep(50);
    for (uint16_t i = 0; i < strips.numPixels(); i++) {
      strips.setPixelColor(i, 0, 0, 0);
    }
    strips.show();  
    
    delay(delayMS); 
    
    for (uint16_t i = 0; i < strips.numPixels(); i++) {
      strips.setPixelColor(i, 255, 0, 0);
    }
    strips.show();
    
    delay(delayMS);
      
    for (uint16_t i = 0; i < strips.numPixels(); i++) {
      strips.setPixelColor(i, 0, 0, 0);
    }
    strips.show();
  }
}

void colorBlinkGreen(int delayMS, int times) {
  for (int i = 0; i< times; i++)  {
    
        beep(50);
    for (uint16_t i = 0; i < strips.numPixels(); i++) {
      strips.setPixelColor(i, 0, 0, 0);
    }
    strips.show();  
    
    delay(delayMS); 
    
    for (uint16_t i = 0; i < strips.numPixels(); i++) {
      strips.setPixelColor(i, 0, 255, 0);
    }
    strips.show();
    
    delay(delayMS);
      
    for (uint16_t i = 0; i < strips.numPixels(); i++) {
      strips.setPixelColor(i, 0, 0, 0);
    }
    strips.show();
  }
}

void colorUpdate(uint32_t c, uint8_t wait, int pixelsFromStart) {
  for (int i = 0; i < strips.numPixels(); i++) {
    strips.setPixelColor(i, 0, 0, 0);
    strips.show();
  }
  for (int i = 0; i < pixelsFromStart; i++) {
    strips.setPixelColor(i, c);
    strips.show();
  }
}


// -----  SERIAL MAIN-Segment-Display
// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void clearDisplay()
{
  s7s.write(0x76);  // Clear display command
}

// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void setBrightness(byte value)
{
  s7s.write(0x7A);  // Set brightness command byte
  s7s.write(value);  // brightness data byte
}

// Turn on any, none, or all of the decimals.
//  The six lowest bits in the decimals parameter sets a decimal 
//  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
//  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
void setDecimals(byte decimals)
{
  s7s.write(0x77);
  s7s.write(decimals);
}
