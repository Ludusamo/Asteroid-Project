#include <TinyWireM.h>
#include <VirtualWire.h>
#include <Adafruit_Sensor.h>

/* Assign a unique ID to this sensor at the same time */
//Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

#define BITS_PER_SECOND 2000
#define OPTICAL_PIN 3
#define TRANSMITTER_PIN 4
#define MAG_ADDRESS 0x1E

void setupTransmitter() {
  vw_set_ptt_inverted(true); // Required for RF Link module
  vw_setup(BITS_PER_SECOND);                 // Bits per sec
  vw_set_tx_pin(TRANSMITTER_PIN);                // pin 4 is used as the transmit data out into the TX Link module, change this as per your needs 
}

void sendMessage(const char* msg) {
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx();                                          // Wait for message to finish
}

void setup(void) 
{
  setupTransmitter();
  
  // Starts TinyWire master library
  TinyWireM.begin();
  setupMagnetometer();
}

//Calculating the magnetic vector sum
float calculateData(float x, float y, float z) {
  return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void sendData(float x, float y, float z, int optical_value) {
  char msg[4];
  dtostrf(calculateData(x, y, z), 4, 4, msg);
  msg[3] = 'm';
  sendMessage(msg);
  dtostrf(optical_value, 4, 4, msg);
  msg[3] = 'd';
  sendMessage(msg);
}

void loop(void) 
{
  // Gets the X, Y, and Z component
    int x, y, z;
    setupMagnetometer();
    TinyWireM.beginTransmission(MAG_ADDRESS);
    TinyWireM.send(0x03);
    TinyWireM.endTransmission();
    
    TinyWireM.requestFrom(MAG_ADDRESS, 6);
    if (6 <= TinyWireM.available()) {
      x = TinyWireM.receive() << 8;
      x |= TinyWireM.receive(); 
      y = TinyWireM.receive() << 8;
      y |= TinyWireM.receive();
      z = TinyWireM.receive() << 8;
      z |= TinyWireM.receive();
    }
    int optical_value = 4800 / (analogRead(OPTICAL_PIN) - 20);
    //int optical_value;
    //optical_value = 100;
    
    sendData(x, y, z, optical_value);
}

void setupMagnetometer() {
  TinyWireM.beginTransmission(MAG_ADDRESS);
  TinyWireM.send(0x02);
  TinyWireM.send(0x00);
  TinyWireM.endTransmission();
}
