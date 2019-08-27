/*
  Time, Temperature and Humidity
  time_temp_humid.ino
  Displays results on Serial Monitor
  Uses AM2320 I2C Temperature and Humidity sensor
  Uses DS1307 Real Time Clock
  Uses TimeLib Library
  Uses DS1307RTC Library
  Uses Adafruit AM2320 Library

  DroneBot Workshop 2019
  https://dronebotworkshop.com
*/

// Include required libraries
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Adafruit_AM2320.h>

// Define constant for RTC I2C Address
#define DS1307_CTRL_ID 0x68

// Define object am2320
Adafruit_AM2320 am2320 = Adafruit_AM2320();

// Variables to count square wave pulses
int ticks = 0;
int old_tick_value = 0;

// Variables for Humidity and Temperature
float h;
float t;

void setup() {
  Serial.begin(9600);

  // Attach Interrupt to pin D2
  attachInterrupt(0, handleInt, FALLING);

  // Set Square Wave at 1Hz
  setSQW(0x10);

  // Initialize Temp & Humid Sensor
  am2320.begin();

  delay(2000);

  // Read Humidity
  h = am2320.readHumidity();
  // Read temperature as Celsius
  t = am2320.readTemperature();
}

void loop() {

  // Read Temp Humid sensor every 10 seconds
  if (ticks == 10) {
    // Read Humidity
    h = am2320.readHumidity();
    // Read temperature as Celsius
    t = am2320.readTemperature();
    ticks = 0;

  }

  // Update serial monitor display if a second has elapsed
  if (ticks != old_tick_value) {
    old_tick_value = ticks;
    printCurrentTime(h, t);
  }

}

// Interrupt Handler
void handleInt() {
  ticks++;

}

// Setup RTC Module for 1 Hz square wave
void setSQW(uint8_t value) {
  Wire.beginTransmission(DS1307_CTRL_ID);
  Wire.write(7);
  Wire.write(value);
  Wire.endTransmission();
}

// Format numbers as 2-digit numbers
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.print('0');
  }
  Serial.print(number);
}

// Print to the serial monitor
void printCurrentTime(float hum, float tem) {
  tmElements_t tm;

  if (RTC.read(tm)) {
    print2digits(tm.Hour);
    Serial.print(':');
    print2digits(tm.Minute);
    Serial.print(':');
    print2digits(tm.Second);
    Serial.print(" - ");
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.print('-');
    print2digits(tm.Month);
    Serial.print('-');
    print2digits(tm.Day);
    Serial.print(" Hum: ");
    Serial.print(hum);
    Serial.print("% - Temp: ");
    Serial.print(tem);
    Serial.print(" C");

    Serial.println();
  }
}
