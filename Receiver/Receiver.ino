#include <VirtualWire.h>    // you must download and install the VirtualWire.h to your hardware/libraries folder
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
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
//Defining object for display
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//File name for data log 
const char filename[] = "datalog.txt";


//SD card chip
const int chipSelect = 10;

//Defining variables for timing
float currentTime, lastTime, elapsedTime;

int numDataPoints = 0;

//Defining variables to hold data
String magVecSum, dis;
int nextRead = 0;

void newCell() {
  Serial.print("   "); 
}

class vector2f {
public:
  float x, y;
  
  vector2f() : x(0), y(0) {}
  vector2f(float x, float y) : x(x), y(y) {}
};

vector2f points[MAX_NUM_POINTS];

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
      output += ((points[j].y - points[i].y) / (points[j].x - points[i].x));
      numSlopes++;
    }
  }
  
  output /= numSlopes;
  return output; 
}

//void elimOutliers() {
//  int currentIndex = 0;
//  for (int i = 0; i < numDataPoints; i++) {
//     if (points[i].x <= 30) {
//       finalPoints[currentIndex] = points[i];
//       currentIndex++;
//     } 
//  }
//}

//Function for converting ASCII integers to a string object
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
    Serial.println("Serial Port has started.");
    
    //Starts the wire library
    Wire.begin();
        
    //Initializes the LCD monitor
    lcd.begin(16, 2); //WIDTH,HEIGHT in characters

// Initialise the IO and ISR
    vw_set_ptt_inverted(true);    // Required for RX Link Module
    vw_setup(2000);                   // Bits per sec
    vw_set_rx_pin(4);           // We will be receiving on pi n 4 i.e te RX pin from the module connects to this pin. 
    vw_rx_start();                      // Start the receiver 
    
    // Debug Information
    Serial.println("Program Start:");
    Serial.println();
    Serial.println("Magnitude of Vectors   Distance (cm)");
    Serial.println();
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

//Function for receiving the message
void receiveMessage() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) {   
    Serial.print(asciiToString(buf, buflen) + " ");
    newCell();
    if (nextRead == 0) {
      magVecSum = asciiToString(buf, buflen);
      nextRead = 1; 
    } else {
      dis = asciiToString(buf, buflen);
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
    Serial.println("Logging");
    
    //Opens the file on the SD card
    File dataFile = SD.open("datalog", FILE_WRITE);

    String distance, magnetic;

    distance = dis.substring(0,6);
    magnetic = magVecSum.substring(0, 6);

    // if the file is available, write to it:
    if (stof(distance) <= 30) {
      if (dataFile) {
        dataFile.println(distance + "   " + magnetic);
        Serial.println(magVecSum.substring(0, 6));
        dataFile.close();
        // print to the serial port too:
        Serial.println(magVecSum + "   " + dis);
      }  
      // if the file isn't open, pop up an error:
      else {
        Serial.println("error opening datalog.txt");
      }
     
      // Appends to the array of points
      points[numDataPoints] = vector2f(stof(distance), stof(magnetic));
      numDataPoints++;
    }
  }
  
  // LCD Printing
  lcd.setCursor(0, 1);
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (buttons & BUTTON_UP) {
      lcd.print("Mag Vec Sum:");
      lcd.setCursor(0, 1);
      lcd.print(magVecSum);
      lcd.setBacklight(RED);
    }
    if (buttons & BUTTON_DOWN) {
      lcd.print("Timer:");
      lcd.setCursor(0, 1);
      lcd.print(currentTime - lastTime);
      lcd.setBacklight(TEAL);
    }
    if (buttons & BUTTON_LEFT) {
      lcd.print("Distance:");
      lcd.setCursor(0, 1);
      lcd.print(dis);
      lcd.setBacklight(GREEN);
    }
  }
}
