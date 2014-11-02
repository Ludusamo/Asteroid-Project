#include <VirtualWire.h>    // you must download and install the VirtualWire.h to your hardware/libraries folder
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <SD.h>

#undef int
#undef abs
#undef double
#undef float
#undef round

// Constants for COLORS
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
#define DELAY 500

#define MAX_NUM_POINTS 1000

//File name for data log 
const char filename[] = "datalog.txt";


//SD card chip
const int chipSelect = 10;

//Defining variables for timing
float currentTime, lastTime, elapsedTime;

int xPoints[1000];
int yPoints[1000];
int finalXPoints[1000];
int finalYPoints[1000];
int numDataPoints = 0;

//Defining variables to hold data
String magVecSum, dis;
int nextRead = 0;

void newCell() {
  Serial.print("   "); 
}

float stof(String string) {
  char bufferArray[32];
  string.toCharArray(bufferArray, sizeof(bufferArray));
  float output = atof(bufferArray);
  return output;
}

float calculateSlope() {
  int numSlopes = 0;
  float output = 0;
  for (int i = 0; i < numDataPoints; i++) {
    for (int j = i + 1; j < numDataPoints; j++) {
      output += ((yPoints[j] - yPoints[i]) / (xPoints[j] - xPoints[i]));
      numSlopes++;
    }
  }
  
  output /= numSlopes;
  return output; 
}

void elimOutliers() {
  int currentIndex = 0;
  for (int i = 0; i < numDataPoints; i++) {
     if (xPoints[i] <= 30) {
       finalXPoints[currentIndex] = xPoints[i];
       finalYPoints[currentIndex] = yPoints[i];
       currentIndex++;
     } 
  }
}

//Function for converting ASCII integers to a string object
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

void setup() {
    Serial.begin(9600);
    Serial.println("Serial Port has started.");
    
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    
    //Starts the wire library
    Wire.begin();

// Initialise the IO and ISR
    vw_set_ptt_inverted(true);    // Required for RX Link Module
    vw_setup(2000);                   // Bits per sec
    vw_set_rx_pin(4);           // We will be receiving on pi n 4 i.e te RX pin from the module connects to this pin. 
    vw_rx_start();                      // Start the receiver 
    
    lastTime = millis();
    
    // Initializes the Chip Select
    Serial.print("Initializing SD card...");
    // make sure that the default chip select pin is set to
    // output, even if you don't use it:
    pinMode(10, OUTPUT);
    
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      return;
    }
    Serial.println("Card Initialized.");
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

void loop() {
  receiveMessage();//Constantly receiving messages
  // Timing
  currentTime = millis();
  if (currentTime - lastTime >= DELAY) {
    lastTime = currentTime;
    Serial.println();
    nextRead = 0;
    
    //Opens the file on the SD card
    File dataFile = SD.open("datalog", FILE_WRITE);

    String distance, magnetic;

    distance = dis.substring(0,3);
    magnetic = magVecSum.substring(0, 3);
    
    Serial.println("d" + distance);
    Serial.println("m" + magnetic);

    // if the file is available, write to it:
//    if (stof(distance) <= 30) {
//      if (dataFile) {
//        dataFile.println(distance + "   " + magnetic);
//        Serial.println(magVecSum.substring(0, 6));
//        dataFile.close();
//        // print to the serial port too:
//        Serial.println(magVecSum + "   " + dis);
//      }  
//      // if the file isn't open, pop up an error:
//      else {
//        Serial.println("error opening datalog.txt");
//      }
//     
//      // Appends to the array of points
////      xPoints[numDataPoints] = stof(distance);
////      yPoints[numDataPoints] = stof(magnetic);
//      numDataPoints++;
//    }
  }
}
