#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS 1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
int tempVal;
void onBeatDetected()
{
  Serial.println("Beat!");
}

LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup()
{
  lcd.init();
  // Turn on the backlight of LCD display.
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shreya BioMetrics");
  Serial.begin(115200);
  Serial.print("Initialising pulse oximeter...");
  if (!pox.begin())
  {
    Serial.println("Failed");
    for (;;)
      ;
  }
  else
  {
    Serial.println("Success");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    Serial.print("Heart Rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm/ spo2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    tsLastReport = millis();
    lcd.setCursor(0, 1);
    lcd.print("HR:");
    lcd.print(pox.getHeartRate());
    lcd.setCursor(9, 1);
    lcd.print("SP:");
    lcd.print(pox.getSpO2());
    lcd.print("%");
    tempVal = analogRead(A2);
    tempVal *= 5;
    tempVal /= 10;
    tempVal -= 6;
    Serial.print("temp:");
    Serial.println(tempVal);
    if (tempVal > 35)
    {
      lcd.setCursor(0, 1);
      lcd.print("Temp:");
      lcd.print(tempVal);
    }
  }
}
