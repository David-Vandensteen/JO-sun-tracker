#include <Arduino.h>
#include <FIR.h>

FIR avg005(5);
FIR avg050(50);
FIR avg100(100);

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("instant:");
  Serial.println("avg005:");
  Serial.println("avg050:");
  Serial.println("avg100:");
}

void loop() {
  int sensorValue = analogRead(A0);
  int sensor005 = sensorValue, sensor050 = sensorValue, sensor100 = sensorValue;

  sensor005 = avg005.updateOutput(sensor005);
  sensor050 = avg050.updateOutput(sensor050);
  sensor100 = avg100.updateOutput(sensor100);

  Serial.print("instant:"); Serial.println(sensorValue);
  Serial.print("avg005:"); Serial.println(sensor005);
  Serial.print("avg050:"); Serial.println(sensor050);
  Serial.print("avg100:"); Serial.println(sensor100);

  delay(1000);
}
