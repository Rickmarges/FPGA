/*
** Authors: Maryn Kaspers and Rick Margés
** This set-up will be used to automatically run the Google Chrome Dino Jump game
** It will use one LDR to recognize the cacti and the birds.
** And another LDR to check the background color because of the theme switches in the game.
** If the data from the LDR, which checks the foreground, is 1.4 times higher than the background: jump.
** The delay set before jumping will be set by a FPGA.
** Because of it's build in clock it will be able to send the correct delay to the Arduino.
** This is needed beacuse the game will run faster and faster as you progress.
*/

const int keyboardPin = 5;					        // Set the keyboardPin to digital pin 5
const int backgroundLDR = A0;				        // The background LDR will be connected to analog pin A0
const int foregroundLDR = A5;				        // The foreground LDR will be connected to analog pin A5
int jumpDelay;                              // A delay set before jumping
int pendingJumps;                           // The amount of jumps pending
unsigned long currentMillis = 0;            // The current time in milliseconds
unsigned long previousJumpMillis = 0;       // The timestamp of the last jump in milliseconds
unsigned long previousDetectionMillis = 0;  // The timestamp of the last detection in milliseconds

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(10);				            // This sets the timeout for the serial to 1. Otherwise the parseInt function is to slow
  pinMode(keyboardPin, OUTPUT);		          // Set the lightPin (will become keyboard) to output
  pinMode(LED_BUILTIN, OUTPUT);             // Visual test with build in LED
  pinMode(backgroundLDR, INPUT);	          // Make sure the LDR will be seen as input
  pinMode(foregroundLDR, INPUT);	          // Also set the other LDR as input
  digitalWrite(LED_BUILTIN, LOW);           // Make sure the built in LED is set at low
}

void loop()
{
  currentMillis = millis();

  // Check if there is incoming serial data to determine the jump delay
  if (Serial.available() > 0) {
    int tempDelay = Serial.parseInt();      // This reads the input data and converts it to an int
    if (tempDelay > 0) {                    // The temp value is used because serial data also returns extra zeros
      jumpDelay = tempDelay;                // If the temp value is greater than 0, set jump delay
      Serial.println(jumpDelay);            // Just to check the incoming data
    }
  }

  detect();                                 // Run the detect function
  jump();                                   // Run the jump function
}

void detect() {
  // Only check for cacti once every tenth of a second so it won't flood pending jumps
  if (currentMillis - previousDetectionMillis >= 100) {
    // Use the 2 LDR's to check for cacti
    // The LDR which check the foreground has to be 1.4 times higher or lower than the background
    // This value is a threshold which will be exceded by the passing cacti
    if (analogRead(foregroundLDR) > analogRead(backgroundLDR) * 1.4
        || analogRead(foregroundLDR) * 1.4 < analogRead(backgroundLDR)) {
      pendingJumps++;                           // If a cactus is registerd increase pending jumps
      previousDetectionMillis = currentMillis;  // Set the previous detection time to the current time
    }
  }
}

void jump() {
  // The dino only has to jump when there are jump pending and
  // the output pin for the jump has to be low in order to jump
  if (pendingJumps > 0 && digitalRead(LED_BUILTIN) == 0) {
    if (currentMillis - previousJumpMillis >= jumpDelay) {
      digitalWrite(keyboardPin, HIGH);          // Set the keyboardPin to high so the dino will jump
      digitalWrite(LED_BUILTIN, HIGH);          // Visual test with the built in led
      previousJumpMillis += jumpDelay;          // Set the time of the previous jump to the current time
      pendingJumps--;                           // Decrease the amount of pending jumps
    }
  } else if (digitalRead(LED_BUILTIN) == 1) {   // If statement to check if the jump pin is high
    if (currentMillis - previousJumpMillis >= 250) {
      digitalWrite(keyboardPin, LOW);           // Set the keyboardPin to low so the dino will go down
      digitalWrite(LED_BUILTIN, LOW);           // Visual test with the built in led
      previousJumpMillis += 100;                // Increase the previous jump timer with 100 which is the amount the pin was high
    }
  }
}
