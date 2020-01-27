/**
 * Two Channel Receiver
 * Author: Shawn Hymel (SparkFun Electronics)
 * Date: Aug 24, 2017
 * 
 * Connect a L298N and RC (PWM) receiver to the Arduino.
 * Mixes two channels for arcade drive.
 */

// Controller pins
const int CH_1_PIN = 9;
const int CH_2_PIN = 10;

// Motor driver pins
const int AIN1_PIN = 7;
const int AIN2_PIN = 6;
const int APWM_PIN = 5;
const int BIN1_PIN = 4;
const int BIN2_PIN = 2;
const int BPWM_PIN = 3;

// Parameters

const int deadzone = 20;  // Anything between -20 and 20 is stop

void setup() {
  // Configure pins
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(APWM_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
  pinMode(BPWM_PIN, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  // Read pulse width from receiver
  int y = pulseIn(CH_2_PIN, HIGH, 25000);
  int x = pulseIn(CH_1_PIN, HIGH, 25000);
  // Convert to PWM value (-255 to 255)
  y = pulseToPWM(y);
  x = pulseToPWM(x);
  // Mix for arcade drive
  int left = y + x;
  int right = y - x;
  // Drive motor
  drive(left, right);
  //Serial.println(left);
  //Serial.println(right);
  delay(5);
}

// Positive for forward, negative for reverse
void drive(int speed_a, int speed_b) {
  // Limit speed between -255 and 255
  speed_a = constrain(speed_a, -255, 255);
  speed_b = constrain(speed_b, -255, 255);
  // Set direction for motor A
  if ( speed_a == 0 ) {
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, LOW);
  } else if ( speed_a > 0 ) {
    digitalWrite(AIN1_PIN, HIGH);
    digitalWrite(AIN2_PIN, LOW);
  } else {
    digitalWrite(AIN1_PIN, LOW);
    digitalWrite(AIN2_PIN, HIGH);
  }
  // Set direction for motor B
  if ( speed_b == 0 ) {
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, LOW);
  } else if ( speed_b > 0 ) {
    digitalWrite(BIN1_PIN, HIGH);
    digitalWrite(BIN2_PIN, LOW);
  } else {
    digitalWrite(BIN1_PIN, LOW);
    digitalWrite(BIN2_PIN, HIGH);
  }
  // Set speed
  analogWrite(APWM_PIN, abs(speed_a));
  analogWrite(BPWM_PIN, abs(speed_b));
}

// Convert RC pulse value to motor PWM value
int pulseToPWM(int pulse) {
   // If we're receiving numbers, convert them to motor PWM
  if ( pulse > 990 ) {
    pulse = map(pulse, 990, 1987, -500, 500);
    pulse = constrain(pulse, -255, 255);
  } else {
    pulse = 0;
  }
  // Anything in deadzone should stop the motor
  if ( abs(pulse) <= deadzone ) {
    pulse = 0;
  }
  return pulse;
}
