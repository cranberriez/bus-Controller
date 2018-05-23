/* By Jacob Vilevac */

#include <Servo.h>

//User configuration:
int percent = 0;  // between -100 and 100, indicates boot speed

int motorPWM[] = {1500, 1500};
int pins[] = {5, 6}; // Left Motor, Right Motor  (L:5, R:6)
int maxSpeed = 40;
int leftMotor = 0;
int rightMotor = 0;
int motors[] = {leftMotor, rightMotor};

const int arraySize = sizeof(pins)/sizeof(int);
Servo controllers[arraySize];

int pwm(int input) {
  // int result = input * 5 + 1500;
  int result = input * 20 + 1500;
  return result;
}

void motor(String input) {
  if (input.equals("forward")) {
    if (leftMotor != rightMotor) { // If we're turning at all (motors not going same speed)
      if (leftMotor > rightMotor) { // Set the slower motor to go as fast as the faster motor
        rightMotor = leftMotor;
      }
      else {
        leftMotor = rightMotor;
      }
    }
    else { // Increase speed by 1
      rightMotor += 1;
      leftMotor += 1;
    }
  }
  else if (input.equals("back")) {
    if (leftMotor != rightMotor) { // If we're turning at all (motors not going same speed)
      // Set the slower motor to go as fast as the faster motor (because they are negatives the faster motor is less than the slower motor)
      if (leftMotor < rightMotor) rightMotor = leftMotor;
      else leftMotor = rightMotor;
    }
    else { // Decrease speed by 1
      rightMotor -= 1;
      leftMotor -= 1;
    }
  }
  else if (input.equals("left")) { // Movement is like turning the wheel of a car
    if (rightMotor < 0 || leftMotor < 0) rightMotor -= 1; // Reversing we want negative because speed is negative
    else rightMotor += 1;
  }
  else if (input.equals("right")) {
    if (rightMotor < 0 || leftMotor < 0) leftMotor -= 1;
    else leftMotor += 1;
  }
  else if (input.equals("stop")) {
    rightMotor = 0;
    leftMotor = 0;
  }

  for (int i=0; i<2; i++)
    motorPWM[i] = pwm(motors[i]);
}

void handleInput(String input) {
  if (input.equals("w")) motor("forward");
  else if (input.equals("a")) motor("left");
  else if (input.equals("d")) motor("right");
  else if (input.equals("s")) motor("backward");
  else if (input.equals("ex")) motor("stop"); // X
  else if (input.equals("sq")); // Square
  else if (input.equals("ci")); // Circle
  else if (input.equals("tr")); // Triangle
  else if (input.equals("select")); // Select
  else if (input.equals("start")); // Start
}

void setup() {
  Serial1.begin(9600);
  Serial1.println("Ready For Input\n");
  controllers[0].attach(pins[0]);
  controllers[1].attach(pins[1]);
  delay(1000);
  Serial1.println("Input Command and Output PWM\n");
}

void loop() {

  for (int i=0; i<2; i++)
    controllers[i].writeMicroseconds(motorPWM[i]);

  if (Serial1.available() > 1) {
    String input = Serial1.readString();
    handleInput(input);
  }
}
