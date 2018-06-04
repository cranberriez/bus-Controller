/* By Jacob Vilevac */

#include <Servo.h>

Servo stopservo;

//User configuration:
int percent = 0;  // between -100 and 100, indicates boot speed

int ledPins[] = {1,3,4};
int pins[] = {5, 6}; // Left Motor, Right Motor  (L:5, R:6)
int maxSpeed = 40;
int leftMotor = 0;
int rightMotor = 0;
int motors[] = {leftMotor, rightMotor};

int servo = 1;
int signsOut = 0;
int delay_time = 230;

const int arraySize = sizeof(pins)/sizeof(int);
Servo controllers[arraySize];

void setup() {
  stopservo.attach(13);
  
  Serial.begin(9600);
  Serial.println("Ready For Input\n");
  controllers[0].attach(pins[0]);
  controllers[1].attach(pins[1]);
  delay(1000);
  Serial.println("Input Command and Output PWM\n");
}

void loop() {
  if (servo == 1) {
    moveSigns();
  }
  
  // Motor writing and movement
  controllers[0].writeMicroseconds(pwm(leftMotor));
  controllers[1].writeMicroseconds(pwm(rightMotor));

  // If i don't get to this, make one of the handle inputs set a variable of the leds to 1 or 2 or whatever
  // And here do an if statement for if the variable is something, do something with the lights

  // Bluetooth and serial input handling
  if (Serial.available()) {
    int input = Serial.read();
    if (input != 13) {
      Serial.println(input);
      handleInput(input);
    }
  }
}

int pwm(int input) {
  int result = input * 5 + 1500;
  return result;
}

void moveSigns() {
  if (signsOut == 0) { // If signs are closed
    signsOut = 1;
    servo = 0;
    stopservo.write(91);
    delay(delay_time);
    stopservo.write(1);
    delay(delay_time);
    stopservo.write(91);
  }
  
  else if (signsOut == 1) { // If signs are open (else if)
    signsOut = 0;
    servo = 0;
    stopservo.write(91);
    delay(delay_time);
    stopservo.write(181);
    delay(delay_time);
    stopservo.write(91);
  }
}

void motor(String input) {
  if (input.equals("forward")) {
    if (leftMotor != rightMotor) { // If we're turning at all (motors not going same speed)
      if (leftMotor > rightMotor) { // Set the slower motor to go as fast as the faster motor
        if (rightMotor <= -1 && leftMotor > 0) {
          leftMotor = 1;
          rightMotor = 1;
        }
        else {
          leftMotor = rightMotor + 1;
          rightMotor = leftMotor;
        }
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
  else if (input == 113) servo = 1; // Square q
  else if (input == 99); // Circle c
  else if (input == 116); // Triangle t
  else if (input == 108); // Select l
  else if (input == 114); // Start r
  else if (input == 13);
  else Serial.println("Broken");
}
