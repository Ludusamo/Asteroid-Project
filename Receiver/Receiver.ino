#include <VirtualWire.h>    // you must download and install the VirtualWire.h to your hardware/libraries folder
#undef int
#undef abs
#undef double
#undef float
#undef round

void setup() {
    Serial.begin(9600);    

// Initialise the IO and ISR
    vw_set_ptt_inverted(true);    // Required for RX Link Module
    vw_setup(2000);                   // Bits per sec
    vw_set_rx_pin(4);           // We will be receiving on pi n 4 i.e the RX pin from the module connects to this pin. 
    vw_rx_start();                      // Start the receiver 
    
    Serial.println("Program Start:");
}

void loop() {
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // check to see if anything has been received
    {
    int i;
     // Message with a good checksum received.
        
    for (i = 0; i < buflen; i++)
    {
        Serial.print(buf[i]);                     // the received data is stored in buffer
        }
    Serial.println("");
     }
}
