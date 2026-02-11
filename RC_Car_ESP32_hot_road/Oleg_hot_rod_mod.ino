#include <GyverMotor.h>

#include <Servo.h>
#include <PS2X_lib.h>
PS2X ps2x;
Servo servo;

GMotor motor1(DRIVER2WIRE, 2, 3, HIGH);

#define PS2_DAT A0
#define PS2_CMD A1
#define PS2_SEL A2
#define PS2_CLK A3

#define brakeLED 6
#define headLIGHT 7

boolean hlight_state = 0;
unsigned long last_time;

void setup() {
  motor.setSmoothSpeed(40);
  motor.setMode(FORWARD);
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, false, false);
  servo.attach(5);
  pinMode(brakeLED, OUTPUT);
  pinMode(headLIGHT, OUTPUT);
}

void loop() {
  delay(10);
  ps2x.read_gamepad(false, 0);
  ps2x.reconfig_gamepad();
  int accel = map(ps2x.Analog(PSS_LY), 0, 256, -255, 255);
  motor.smoothTick(accel);
  int steering = map(ps2x.Analog(PSS_RX), 0, 256, 55, 130);
  servo.write(steering);
  if (ps2x.Button(PSB_CROSS) && millis() - last_time > 150) {
    last_time = millis();
    hlight_state = !hlight_state;
    digitalWrite(headLIGHT, hlight_state);
  }
  if (accel == 0) {
    digitalWrite(brakeLED, HIGH);
  }
  else {
    digitalWrite(brakeLED, LOW);
  }
}

