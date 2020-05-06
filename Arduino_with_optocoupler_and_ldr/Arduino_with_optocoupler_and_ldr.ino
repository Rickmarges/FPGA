/*
** This set-up will be used to automatically run the Google Chrome Dino Jump game
** It will use one LDR to recognize the cacti and the birds.
** And another LDR to check the background color because of the theme switches in the game.
** If the data from the LDR, which checks the foreground, is 1.4 times higher than the background: jump.
** As of now there is a LED in place where a keyboard should be.
** This is because it's easier to test this way.
** Once everything is IRL the LED will be replaced with a keyboard to make the necessary jumps.
** The delay set before jumping will be set by a FPGA.
** Because of it's build in clock it will be able to send the correct delay to the Arduino.
** This is needed beacuse the game will run faster and faster as you progress.
*/

int lightPin = 5;					        // Set the lightPin (will become keyboard) to digital pin 5
int backgroundLDR = A0;				    // The background LDR will be connected to analog pin A0
int foregroundLDR = A5;				    // The foreground LDR will be connected to analog pin A5
int jumpDelay;                    // A delay set before jumping

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(1);				      // This sets the timeout for the serial to 1. Otherwise the parseInt function is to slow
  pinMode(lightPin, OUTPUT);		    // Set the lightPin (will become keyboard) to output
  pinMode(13, OUTPUT);              // Visual test with build in LED
  pinMode(backgroundLDR, INPUT);	  // Make sure the LDR will be seen as input
  pinMode(foregroundLDR, INPUT);	  // Also set the other LDR as input
  digitalWrite(13, LOW);            // Make sure the build in LED is set at low
}

void loop()
{
  // Check if there is incoming serial data to determine the jump delay
  if (Serial.available() > 0) {
    int tempDelay = Serial.parseInt(); // This reads the input data and converts it to an int
    if (tempDelay > 0) {            // The temp value is used because serial data also returns extra zeros
      jumpDelay = tempDelay;        // If the temp value is greater than 0, set jump delay
      Serial.println(jumpDelay);    // Just to check the incoming data
    }
  }

  // The if statement checks if the forground LDR is 1.4 times higher or lower than the background because of cacti with different themes.
  if (analogRead(foregroundLDR) > analogRead(backgroundLDR) * 1.4 || analogRead(foregroundLDR) * 1.4 < analogRead(backgroundLDR)) {
    delay(jumpDelay);					      // This sets the delay before jumping
    Serial.println(jumpDelay); 		  // Just to check the incoming data
    digitalWrite(lightPin, HIGH);	  // Set the light pin to high so the dino will jump
    digitalWrite(13, HIGH);         // Visual test with the built in led
    delay(250);							        // Use short delay before the pin will be set back to low
    digitalWrite(lightPin, LOW);	  // Set the pin back to low so the dino will go down
    digitalWrite(13, LOW);
  }

  delay(10); // This short delay makes the code run smoother
}
