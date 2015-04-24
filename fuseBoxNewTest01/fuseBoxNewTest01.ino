int ledPin1 = 7; // choose the pin for the LED
int ledPin2 = 6; // choose the pin for the LED
int ledPin3 = 5; // choose the pin for the LED
int ledPin4 = 4; // choose the pin for the LED
int ledPin5 = 3; // choose the pin for the LED
int ledPin6 = 2; // choose the pin for the LED

int inPin1 = 22;   // choose the input pin (for a pushbutton)
int inPin2 = 24;   // choose the input pin (for a pushbutton)
int inPin3 = 26;   // choose the input pin (for a pushbutton)
int inPin4 = 28;   // choose the input pin (for a pushbutton)
int inPin5 = 30;   // choose the input pin (for a pushbutton)
int inPin6 = 32;   // choose the input pin (for a pushbutton)
int val = 0;     // variable for reading the pin status

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);  // declare LED as output
  pinMode(ledPin2, OUTPUT);  // declare LED as output
  pinMode(ledPin3, OUTPUT);  // declare LED as output
  pinMode(ledPin4, OUTPUT);  // declare LED as output
  pinMode(ledPin5, OUTPUT);  // declare LED as output
  pinMode(ledPin6, OUTPUT);  // declare LED as output

  pinMode(inPin1, INPUT_PULLUP);    // declare pushbutton as input
  pinMode(inPin2, INPUT_PULLUP);    // declare pushbutton as input
  pinMode(inPin3, INPUT_PULLUP);    // declare pushbutton as input
  pinMode(inPin4, INPUT_PULLUP);    // declare pushbutton as input
  pinMode(inPin5, INPUT_PULLUP);    // declare pushbutton as input
  pinMode(inPin6, INPUT_PULLUP);    // declare pushbutton as input
}

void loop() {

  val = digitalRead(inPin1);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(ledPin1, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin1, HIGH);  // turn LED ON
  }

  val = digitalRead(inPin2);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(ledPin2, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin2, HIGH);  // turn LED ON
  }

  val = digitalRead(inPin3);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(ledPin3, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin3, HIGH);  // turn LED ON
  }

  val = digitalRead(inPin4);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(ledPin4, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin4, HIGH);  // turn LED ON
  }

  val = digitalRead(inPin5);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(ledPin5, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin5, HIGH);  // turn LED ON
  }

  val = digitalRead(inPin6);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(ledPin6, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin6, HIGH);  // turn LED ON
  }

}

