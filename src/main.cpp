#include <LiquidCrystal_I2C.h>
#define TRUE 1
#define FALSE 0
#define DEBUG TRUE

#define LCD_PIN 4
#define LIGHT_SENSOR_1_PIN A0
#define LIGHT_SENSOR_2_PIN A1
#define RELAY_1_PIN 12
#define RELAY_2_PIN 13
#define LUMINOSITY_THRESHOLD_PIN A2
#define JACK_BUTTON_1_PIN 7
#define JACK_BUTTON_2_PIN 8

LiquidCrystal_I2C lcd(0x27, 20, LCD_PIN);

void setup_pin() {
  pinMode(LIGHT_SENSOR_1_PIN, INPUT);
  pinMode(LIGHT_SENSOR_2_PIN, INPUT);
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(JACK_BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(JACK_BUTTON_2_PIN, INPUT_PULLUP);
  if (DEBUG) {
    Serial.println("Pin setup completed");
    Serial.print("LIGHT_SENSOR_1_PIN: ");
    Serial.println(LIGHT_SENSOR_1_PIN);
    Serial.print("LIGHT_SENSOR_2_PIN: ");
    Serial.println(LIGHT_SENSOR_2_PIN);
    Serial.print("RELAY_1_PIN: ");
    Serial.println(RELAY_1_PIN);
    Serial.print("RELAY_2_PIN: ");
    Serial.println(RELAY_2_PIN);
    Serial.print("JACK_BUTTON_1_PIN: ");
    Serial.println(JACK_BUTTON_1_PIN);
    Serial.print("JACK_BUTTON_2_PIN: ");
    Serial.println(JACK_BUTTON_2_PIN);
  }
}

void setup_lcd() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Lum1   Lum2  Moy");
  lcd.setCursor(0, 2);
  lcd.print("V1 OFF  V2 OFF");
  lcd.setCursor(0, 3);
  lcd.print("Seuil:");
}

void update_lcd(int lum1Percent, int lum2Percent, int average, int threshold, bool jack1Pressed, bool jack2Pressed) {
  // Lum1
  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print(lum1Percent);
  lcd.print("%  ");
  // Lum2
  lcd.setCursor(7, 1);
  lcd.print("     ");
  lcd.setCursor(7, 1);
  lcd.print(lum2Percent);
  lcd.print("%  ");
  // Average
  lcd.setCursor(13, 1);
  lcd.print("    ");
  lcd.setCursor(13, 1);
  lcd.print(average);
  lcd.print("%  ");

  lcd.setCursor(8, 3);
  lcd.print("   ");
  lcd.setCursor(8, 3);
  lcd.print(threshold);
  lcd.print("%   ");

  lcd.setCursor(0, 2);
  if (jack1Pressed || average > threshold) {
    lcd.print("V1 ON ");
  } else {
    lcd.print("V1 OFF");
  }

  lcd.setCursor(8, 2);
  if (jack2Pressed || average > threshold) {
    lcd.print("V2 ON ");
  } else {
    lcd.print("V2 OFF");
  }
}

void setup() {
  if (DEBUG) {
    Serial.begin(9600);
    Serial.println("Debug mode enabled");
  }
  setup_pin();
  setup_lcd();
}

void loop() {
  bool isJackButton1Pressed = digitalRead(JACK_BUTTON_1_PIN) == LOW;
  bool isJackButton2Pressed = digitalRead(JACK_BUTTON_2_PIN) == LOW;

  int sensorValue1 = analogRead(LIGHT_SENSOR_1_PIN);
  int sensorValue2 = analogRead(LIGHT_SENSOR_2_PIN);

  int lum1Percent = map(sensorValue1, 0, 1023, 0, 100);
  int lum2Percent = map(sensorValue2, 0, 1023, 0, 100);

  int average = (lum1Percent + lum2Percent) / 2;

  int thresholdRaw = analogRead(LUMINOSITY_THRESHOLD_PIN);
  int threshold = map(thresholdRaw, 0, 1023, 0, 100);

  update_lcd(lum1Percent, lum2Percent, average, threshold, isJackButton1Pressed, isJackButton2Pressed);

  if (DEBUG) {
    Serial.print("Lum1: ");
    Serial.print(sensorValue1);
    Serial.print(" (");
    Serial.print(lum1Percent);
    Serial.print("%)  Lum2: ");
    Serial.print(sensorValue2);
    Serial.print(" (");
    Serial.print(lum2Percent);
    Serial.print("%)  Moyenne: ");
    Serial.print(average);
    Serial.print("%  isJackButton1Pressed: ");
    Serial.print(isJackButton1Pressed);
    Serial.print("  isJackButton2Pressed: ");
    Serial.println(isJackButton2Pressed);
  }

  if (isJackButton1Pressed || average > threshold) {
    digitalWrite(RELAY_1_PIN, HIGH);
  } else {
    digitalWrite(RELAY_1_PIN, LOW);
  }
  if (isJackButton2Pressed || average > threshold) {
    digitalWrite(RELAY_2_PIN, HIGH);
  } else {
    digitalWrite(RELAY_2_PIN, LOW);
  }

  delay(500);
}
