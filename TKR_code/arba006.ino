// Пины моторов (как у вас в описании)
#define MOTOR_L_PWM 2
#define MOTOR_L_DIR 3
#define MOTOR_R_PWM 4
#define MOTOR_R_DIR 5

String inputString = "";
boolean stringComplete = false;

void setup() {
  Serial.begin(9600);
  
  // Настройка пинов моторов
  pinMode(MOTOR_L_PWM, OUTPUT);
  pinMode(MOTOR_L_DIR, OUTPUT);
  pinMode(MOTOR_R_PWM, OUTPUT);
  pinMode(MOTOR_R_DIR, OUTPUT);
  
  // Изначально моторы выключены
  stopMotors();
  
  inputString.reserve(50);
  Serial.println("Arduino ready");
}

void loop() {
  if (stringComplete) {
    parseCommand(inputString);
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}

void parseCommand(String cmd) {
  // Формат: "L:speed,R:speed"
  int commaIndex = cmd.indexOf(',');
  if (commaIndex == -1) return;
  
  String leftStr = cmd.substring(2, commaIndex);  // Пропускаем "L:"
  String rightStr = cmd.substring(commaIndex + 3); // Пропускаем ",R:"
  
  int leftSpeed = leftStr.toInt();
  int rightSpeed = rightStr.toInt();
  
  setMotor(MOTOR_L_PWM, MOTOR_L_DIR, leftSpeed);
  setMotor(MOTOR_R_PWM, MOTOR_R_DIR, rightSpeed);
}

void setMotor(int pwmPin, int dirPin, int speed) {
  if (speed > 0) {
    digitalWrite(dirPin, HIGH);
    analogWrite(pwmPin, abs(speed));
  } else if (speed < 0) {
    digitalWrite(dirPin, LOW);
    analogWrite(pwmPin, abs(speed));
  } else {
    analogWrite(pwmPin, 0);
  }
}

void stopMotors() {
  analogWrite(MOTOR_L_PWM, 0);
  analogWrite(MOTOR_R_PWM, 0);
}