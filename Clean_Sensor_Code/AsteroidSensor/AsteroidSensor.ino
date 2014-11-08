#include <VirtualWire.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

// Define Adafruit Magnetometer Sensor Object
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// Pin for Analog Optical Pin Read
#define OPTICAL_PIN A3

// Initiate Transmitter function
void initializeTransmitter() {
  vw_set_ptt_inverted(true); // Required for RF Link module
  vw_setup(2000);                 // Bits per sec
  vw_set_tx_pin(8);                // pin 3 is used as the transmit data out into the TX Link module, change this as per your needs 
}

// Function for sending messages
void sendMessage(const char* msg) {
  if (vw_send((uint8_t *)msg, strlen(msg))) Serial.println(msg);
  else Serial.println("I am not sending because I am mad."); 
  vw_wait_tx();                                          // Wait for message to finish
}

void setup(void) 
{
  // Opens Serial Port
  Serial.begin(9600);
  
  // Initiates Transmitter
  initializeTransmitter();
  
  // Initialize the sensor and checks if the sensor is found
  if(!mag.begin()) {
    Serial.println("Magnetometer Not Detected");
    while(1);
  }
  
  Serial.println("New Data: ");
}

// Calculating the magnetic vector sum
float calculateData(float x, float y, float z) {
  return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

/* Sends Data to Receiver
dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf); <---- Parameters for dtostrf function
*/
void sendData(float x, float y, float z, int optical_value) {
  char msg[10];
  dtostrf(calculateData(x, y, z), 4, 4, msg);
  msg[9] = 'm';
  sendMessage(msg);
  dtostrf(optical_value, 4, 4, msg);
  msg[9] = 'd';
  sendMessage(msg);
}

void loop(void) 
{
  // Requests for a magnetometer event
  sensors_event_t event; 
  mag.getEvent(&event);
  
  // Gets the X, Y, and Z component
  float x = event.magnetic.x;
  float y = event.magnetic.y;
  float z = event.magnetic.z;
  
  // Gets optical value and adjusts it as per the calculation specified by vendor [insert source here]
  int optical_value = 4800 / (analogRead(OPTICAL_PIN) - 20);
  
  // Debug prints the data to Serial Monitor
  printData(x, y, z, optical_value);
  
  // Sends Data to Receiver
  sendData(x, y, z, optical_value);
  
  delay(250);
}


/* 
Serial port debug methods
*/
void newCell() {
  Serial.print("   "); 
}

void printData(float x, float y, float z, int optical_value) {
  Serial.print(x); newCell();
  Serial.print(y); newCell();
  Serial.print(z); newCell();
  
  Serial.println(optical_value);
}
