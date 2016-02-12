//defines
#define ONE_WIRE_BUS D2
#define TOKEN "hsFRLnMcucOZlfLsQbH9BRbJpveccOc37ksq7eLOtjztxoEpZDA1D2wnWiuP"
#define UBIVARSIZE 24
#define PUSHFREQ 300
#define FILENAME "holiday_road"
#define MYVERSION "0.8.19"
#define GETTEMPFEQ 15
#define PUSHTOUBIFLAG 1
#define M1 A6
#define M1POWER D5



//Globals
bool debug = true;
  bool gettempflag = true;
  char* ubivar[]={"564a6d137625425cf86b4ce4", "xxx", "xxx","xxx"};
  char resultstr[64];
  int button = D4;
  int buttonvalue = 0;
  int displayMode = 4;
  int deviceCount = 0;
  int lastDeviceCount, lastime, mycounter,thistime, lasttime = 0;
  int prevPos = 0;
  int value = 0;
  int encoderA = A0;
  int encoderB = A1;
  int M1PCT = 0;
  int mydelay = 250;
  int relay = D3;
  float temperature = 0.0;
  int relayHoldDown = 30000;
  int moistureCheckerFreq = 7200;
  volatile bool A_set = false;
  volatile bool B_set = false;
  volatile int encoderPos = 0;

  http_header_t headers[] = {
        { "Content-Type", "application/json" },
        { "X-Auth-Token" , TOKEN },
      { NULL, NULL } // NOTE: Always terminate headers will NULL
  };
  http_request_t request;
  http_response_t response;

//Prototypes
char *formatTempToBody(float temperature, int tempIndex);
  String convertMillisToHuman(int ms);
  void debugSerial(int i );
  void dispatchEncoder();
  void doEncoderA();
  void doEncoderB();
  void expireRelay();
  double freqChecker();
  int getDeviceCount();
  void oDispatch(int tempIndex, float mytemp);
  void printAddress(DeviceAddress deviceAddress);
  int printEEPROMFunc(String command);
  int queryDevices(String command);
  int regDevice(String command);
  int regDeviceFunc(String command);
  Timer relayTimer(relayHoldDown, expireRelay);
  int relayFunc(String command);
  int setModeFunc(String command);
  int setmode(String command);
  void temperatureJob();


//Declarations
MicroOLED oled;
 OneWire oneWire(ONE_WIRE_BUS);
 DallasTemperature sensor(&oneWire);
 HttpClient http;

   //devices
    // encolusre address   deviceIndexArray[0]:  28 7E F7 25 03 00 00 77
    DeviceAddress deviceIndexArray[5];  //dynamic Array
   //  DeviceAddress outsideAddress = { 0x28, 0xe, 0x52, 0x58, 0x6, 0x0, 0x0, 0xe };
    //DeviceAddress floorAddress = { 0x28, 0x56, 0xB1, 0x3A, 0x06, 0x00, 0x00, 0x82 };
    //DeviceAddress pitAddress = { 0x28, 0x31, 0x26, 0x59, 0x06, 0x00, 0x00, 0x3A };
    DeviceAddress boardAddress = { 0x28, 0x49, 0x2E, 0xE3, 0x02, 0x00, 0x00, 0x29 };
    DeviceAddress*  deviceAddressArray[1] =  { &boardAddress } ;
    //String deviceNames[4]= { "out", "flr", "pit", "brd" };
    String deviceNames[1]= { "brd" };



/* Device Addresses
    //insideThermometer = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };
    Device Index 0 28 56 B1 3A 06 00 00 82
    Device Index 1 28 0E 52 58 06 00 00 0E
    Device Index 2 28 31 26 59 06 00 00 3A
    Device Index 3 28 49 2E E3 02 00 00 29

*/

/*  Notes
*
*
*    Hardware Connections:
*    This sketch was written specifically for the Photon Micro OLED Shield, which does all the wiring for you. If you have a Micro OLED breakout, use the following hardware setup:
*
*      MicroOLED ------------- Photon
*        GND ------------------- GND
*        VDD ------------------- 3.3V (VCC)
*      D1/MOSI ----------------- A5 (don't change)
*      D0/SCK ------------------ A3 (don't change)
*        D2 -------------------- D2 oneWire bus
*        D/C ------------------- D6 (can be any digital pin)
*        RST ------------------- D7 (can be any digital pin)
*        CS  ------------------- A2 (can be any digital pin)
*                                A0 encoder PinA
*                                A1 encoder PinB
*
*      A0   ENCODER PinA
*      A1   ENCODER PinB
*      A2   SPI chip select
*      A3   OLED SCK
*      A4
*      A5   OLED MOSI
*      A6   proposed mositure read
*      A7
*      D0
*      D1
*      D2   ONEWIRE bus
*      D3   Relay
*      D4  Encoder Button
*      D5   proposed moisture power
*      D6   OLED D/C
*      D7   OLED reset
*
*
*
*
*/
