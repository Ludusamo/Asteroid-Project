#include <VirtualWire.h>    // you must download and install the VirtualWire.h to your hardware/libraries folder
#undef int
#undef abs
#undef double
#undef float
#undef round

int count = 0;

void newCell() {
  Serial.print("   "); 
}

String asciiToString(uint8_t* buf, int buflen) {
  String str, final = "";
  int x;
  for (int i = 0; i <= buflen; i++) {
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

void setup() {
    Serial.begin(9600);    

// Initialise the IO and ISR
    vw_set_ptt_inverted(true);    // Required for RX Link Module
    vw_setup(2000);                   // Bits per sec
    vw_set_rx_pin(4);           // We will be receiving on pi n 4 i.e the RX pin from the module connects to this pin. 
    vw_rx_start();                      // Start the receiver 
    
    Serial.println("Program Start:");
    Serial.println();
    Serial.println("   Distance (cm)");
    Serial.println();
}

void receiveMessage() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) {   
//    for (int i = 0; i < buflen; i++) {
//      Serial.print(buf[i]);                     // the received data is stored in buffer
//    }
//    asciiToString(buf, buflen);
    Serial.print(asciiToString(buf, buflen) + " ");
    newCell();
    count++;
  }
}

void loop() {
  receiveMessage();
  if (count == 2) {
    count = 0;
    Serial.println(); 
  }
}
