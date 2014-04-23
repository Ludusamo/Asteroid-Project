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

#include <VirtualWire.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// Constants
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define OPTICAL_PIN 0


boolean X, Y, Z, HEADING, ALL;

void displaySensorDetails(void) {
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

void setupTransmitter() {
  vw_set_ptt_inverted(true); // Required for RF Link module
  vw_setup(2000);                 // Bits per sec
  vw_set_tx_pin(3);                // pin 3 is used as the transmit data out into the TX Link module, change this as per your needs 
}

void sendMessage(const char* msg) {
  if (vw_send((uint8_t *)msg, strlen(msg))) Serial.println(msg);
  else Serial.println("I am not sending because I am mad."); 
  vw_wait_tx();                                          // Wait for message to finish
}

void setup(void) 
{
  Serial.begin(9600);
  setupTransmitter();
  
  /* Initialise the sensor */
  if(!mag.begin()) {
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  
  delay(20 * 1000);
  Serial.println("New Data: ");
}

// Formats it so that it will create a new cell when opened in excel
void newCell() {
  Serial.print("   "); 
}

void printData(float x, float y, float z, int optical_value) {
  Serial.print(x); newCell();
  Serial.print(y); newCell();
  Serial.print(z); newCell();
  
  Serial.println(optical_value);
}

float calculateData(float x, float y, float z) {
  return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void sendData(float x, float y, float z, int optical_value) {
  char msg[4];
  dtostrf(calculateData(x, y, z), 4, 4, msg);
  sendMessage(msg);
  dtostrf(optical_value, 4, 4, msg);
  sendMessage(msg);
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
  
  // Gets the X, Y, and Z component
  float x = event.magnetic.x;
  float y = event.magnetic.y;
  float z = event.magnetic.z;
  
  int optical_value = 4800 / (analogRead(OPTICAL_PIN) - 20);
  
  printData(x, y, z, optical_value);
  
  sendData(x, y, z, optical_value);
  
  delay(10 * 1000);
}
