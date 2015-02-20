#include <VirtualWire.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

#define OPTICAL_PIN A3

void setupTransmitter() {
  vw_set_ptt_inverted(true); // Required for RF Link module
  vw_setup(2000);                 // Bits per sec
  vw_set_tx_pin(8);                // pin 3 is used as the transmit data out into the TX Link module, change this as per your needs 
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

//Calculating the magnetic vector sum
float calculateData(float x, float y, float z) {
  return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void sendData(float x, float y, float z, int optical_value) {
  char msg[6];
  dtostrf(calculateData(x, y, z), 6, 6, msg);
  msg[5] = 'm';
  sendMessage(msg);
  dtostrf(optical_value, 6, 6, msg);
  msg[5] = 'd';
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
  
  delay(250);
}
