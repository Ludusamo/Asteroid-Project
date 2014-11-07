#include <VirtualWire.h>    // you must download and install the VirtualWire.h to your hardware/libraries folder
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <SD.h>

#undef int
#undef abs
#undef double
#undef float
#undef round

//Defining variables to hold data
String magVecSum, dis;

// Initializes the Receiver
void initializeReceiver() {
  vw_set_ptt_inverted(true);    // Required for RX Link Module
  vw_setup(2000);                   // Bits per sec
  vw_set_rx_pin(4);           // We will be receiving on pi n 4 i.e te RX pin from the module connects to this pin. 
  vw_rx_start();                      // Start the receiver  
}

// Function for converting strings to floats
float stof(String string) {
  char bufferArray[32];
  string.toCharArray(bufferArray, sizeof(bufferArray));
  float output = atof(bufferArray);
  return output;
}

// Function for converting ASCII integers to a string object
String asciiToString(uint8_t* buf, int buflen) {
  String str, final = "";
  int x;
  for (int i = 0; i <= buflen - 1; i++) {
    str = "";
    str += buf[i];
    x = str.toInt();
    if (x == 46) {
      final += ".";
    } else if (x == 45) {
      final += "-";
    } else if (x != 32) {
      final += x - 48;
    }
  }
  return final;
}

// Function for receiving the message
void receiveMessage() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) {
    for (int i = 0; i < buflen; i++) {
      if (buf[i] == 'm') {
        magVecSum = asciiToString(buf, buflen);
      }
      if (buf[i] == 'd') {
        dis = asciiToString(buf, buflen);
      }  
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Serial Port has started.");
    
  //Starts the wire library
  Wire.begin();

  // Initialise the IO and ISR
  initializeReceiver();
}

void loop() {
  receiveMessage();//Constantly receiving messages
    
  Serial.println("d" + dis);
  Serial.println("m" + magVecSum);
}
