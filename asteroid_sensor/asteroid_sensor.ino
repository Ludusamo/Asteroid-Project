/***************************************************************************
  This is a library example for the HMC5883 magnentometer/compass

  Designed specifically to work with the Adafruit HMC5883 Breakout
  http://www.adafruit.com/products/1746
 
  *** You will also need to install the Adafruit_Sensor library! ***

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries with some heading example from
  Love Electronics (loveelectronics.co.uk)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the version 3 GNU General Public License as
 published by the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <processing,ser

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define OPTICAL_PIN 0


boolean X, Y, Z, HEADING, ALL;
Serial mySerial;
PrintWriter output;

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  
  lcd.begin(16, 2);
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
  
  float x = event.magnetic.x;
  float y = event.magnetic.y;
  float z = event.magnetic.z;
   
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(z); Serial.print("  ");Serial.println("uT");
  
  float heading = atan2(y, x);

  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  
  int optical_value = 4800 / (analogRead(OPTICAL_PIN) - 20);
  
  Serial.print("Optical Value: "); Serial.println(optical_value);
  
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (buttons & BUTTON_UP) {
      X = true;
      Y = false;
      Z = false;
      HEADING = false;
      ALL = false;
    }
    if (buttons & BUTTON_DOWN) {
      X = false;
      Y = false;
      Z = true;
      HEADING = false;
      ALL = false;
    }
    if (buttons & BUTTON_LEFT) {
      X = false;
      Y = true;
      Z = false;
      HEADING = false;
      ALL = false;
    }
    if (buttons & BUTTON_RIGHT) {
      X = false;
      Y = false;
      Z = false;
      HEADING = true;
      ALL = false;
    }
    if (buttons & BUTTON_SELECT) {
      X = false;
      Y = false;
      Z = false;
      HEADING = false;
      ALL = true;
    }
  }
  
  lcd.clear();
  
  if (X) {
    lcd.setBacklight(RED);
    lcd.setCursor(0, 0);
    lcd.print("X: "); lcd.print(x);
  } else if (Y) {
    lcd.setBacklight(YELLOW);
    lcd.setCursor(0, 0);
    lcd.print("Y: "); lcd.print(y);
  } else if (Z) {
    lcd.setBacklight(GREEN);
    lcd.setCursor(0, 0);
    lcd.print("Z: "); lcd.print(z);
  } else if (HEADING) {
    lcd.setBacklight(TEAL);
    lcd.setCursor(0, 0);
    lcd.print("Distance (cm): "); 
    lcd.setCursor(0, 1);
    lcd.print(optical_value);
  } else if (ALL) {
    lcd.setBacklight(VIOLET);
    lcd.setCursor(0, 0);
    lcd.print(x); lcd.print(" "); lcd.print(y); 
    lcd.setCursor(0, 1);
    lcd.print(z); lcd.print(" ");  lcd.print(headingDegrees);
  }
  
  delay(500);
}