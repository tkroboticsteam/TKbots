#include <Wire.h>
//#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const int photoresistorPin = A0; // Пин, к которому подключен фоторезистор

// Значения яркости для трех звезд
const int siriusBrightness = 5; // Яркость Сириуса
const int rigelBrightness = 3; // Яркость Ригила
const int betelgeuseBrightness = 1; // Яркость Бетельгейзе

void setup() {
  Serial.begin(9600); // Инициализируем последовательную связь для вывода значений на монитор порта

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}

void loop() {
  // Считываем значение с фоторезистора
  int lightLevel = analogRead(photoresistorPin);
  int brightnessLevel = map(lightLevel, 0, 1023, 0, 10);

  // Выводим значения на монитор порта
  Serial.print("Light Level: ");
  Serial.println(brightnessLevel);
  
  // Проверяем яркость и выводим информацию о звезде при соответствующей яркости
  if (brightnessLevel >= siriusBrightness) {
    Serial.println("Фоторезистор: Обнаружена звезда Сириус");
    printApproachInfo(30); // Передаем скорость 30 м/с для достижения объекта
  } else if (brightnessLevel >= rigelBrightness) {
    Serial.println("Фоторезистор: Обнаружена звезда Ригель");
    printApproachInfo(20); // Передаем скорость 20 м/с для достижения объекта
  } else if (brightnessLevel >= betelgeuseBrightness) {
    Serial.println("Фоторезистор: Обнаружена звезда Бетельгейзе");
    printApproachInfo(15); // Передаем скорость 15 м/с для достижения объекта
  }

  // Измеряем расстояние и выводим его
  measureDistance(); 

  delay(400); // Задержка для стабилизации
}

// Функция для вывода информации о скорости приближения к объекту
void printApproachInfo(int speed) {
  Serial.print("Для приближения к объекту рекомендуется скорость "); Serial.print(speed); Serial.println(" м/с");
}

// Функция для измерения расстояния
void measureDistance() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) {
    Serial.print("Дальномер: Расстояние до объекта (мм): ");
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" мм");
  } else {
    Serial.println("Дальномер: Объект не обнаружен");
  }
}
