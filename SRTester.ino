/* By Jacob Vilevac */

#include <Servo.h>

//User configuration:
int percent = 0;  // between -100 and 100, indicates boot speed

int pins[] = {5, 6}; // Left Motor, Right Motor  (L:5, R:6)
int maxSpeed = 40;
int leftMotor = 0;
int rightMotor = 0;
int motors[] = {leftMotor, rightMotor};

const int arraySize = sizeof(pins)/sizeof(int);
Servo controllers[arraySize];

void setup() {
  Serial.begin(9600);
  Serial.println("Ready For Input\n");
  controllers[0].attach(pins[0]);
  controllers[1].attach(pins[1]);
  delay(1000);
  Serial.println("Input Command and Output PWM\n");
}

void loop() {
  controllers[0].writeMicroseconds(pwm(leftMotor));
  controllers[1].writeMicroseconds(pwm(rightMotor));
  Serial.println(pwm(leftMotor));

  if (Serial.available()) {
    int input = Serial.read();
    if (input != 13) {
      Serial.println(input);
      handleInput(input);
    }
  }
}

int pwm(int input) {
  // int result = input * 5 + 1500;
  int result = input * 20 + 1500;
  return result;
}

void motor(String input) {
  if (input.equals("forward")) {
    if (leftMotor != rightMotor) { // If we're turning at all (motors not going same speed)
      if (leftMotor > rightMotor) { // Set the slower motor to go as fast as the faster motor
        leftMotor = rightMotor + 1;
        rightMotor = leftMotor;
      }
      else {
        rightMotor = leftMotor + 1;
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
    rightMotor += 1;
    leftMotor -= 1;
  }

  else if (input.equals("right")) {
    leftMotor += 1;
    rightMotor -= 1;
  }

  else if (input.equals("stop")) {
    rightMotor = 0;
    leftMotor = 0;
  }
}

void handleInput(float input) {
  if (input == 119) motor("forward"); // w
  else if (input == 97) motor("left"); // a
  else if (input == 100) motor("right"); // d
  else if (input == 115) motor("back"); // s
  else if (input == 120) motor("stop"); // X x
  else if (input == 113); // Square q
  else if (input == 99); // Circle c
  else if (input == 116); // Triangle t
  else if (input == 108); // Select l
  else if (input == 114); // Start r
  else if (input == 13);
  else Serial.println("Broken");
}
