// This is a basic snapshot sketch using the VC0706 library.
// On start, the Arduino will find the camera and SD card and
// then snap a photo, saving it to the SD card.
// Public domain.

// If using an Arduino Mega (1280, 2560 or ADK) in conjunction
// with an SD card shield designed for conventional Arduinos
// (Uno, etc.), it's necessary to edit the library file:
//   libraries/SD/utility/Sd2Card.h
// Look for this line:
//   #define MEGA_SOFT_SPI 0
// change to:
//   #define MEGA_SOFT_SPI 1
// This is NOT required if using an SD card breakout interfaced
// directly to the SPI bus of the Mega (pins 50-53), or if using
// a non-Mega, Uno-style board.

#include <Adafruit_VC0706.h>
#include <SPI.h>
#include <SD.h>

// comment out this line if using Arduino V23 or earlier
#include <SoftwareSerial.h>         

// uncomment this line if using Arduino V23 or earlier
// #include <NewSoftSerial.h>       

// SD card chip select line varies among boards/shields:
// Adafruit SD shields and modules: pin 10
// Arduino Ethernet shield: pin 4
// Sparkfun SD shield: pin 8
// Arduino Mega w/hardware SPI: pin 53
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
#define chipSelect 10

// Pins for camera connection are configurable.
// With the Arduino Uno, etc., most pins can be used, except for
// those already in use for the SD card (10 through 13 plus
// chipSelect, if other than pin 10).
// With the Arduino Mega, the choices are a bit more involved:
// 1) You can still use SoftwareSerial and connect the camera to
//    a variety of pins...BUT the selection is limited.  The TX
//    pin from the camera (RX on the Arduino, and the first
//    argument to SoftwareSerial()) MUST be one of: 62, 63, 64,
//    65, 66, 67, 68, or 69.  If MEGA_SOFT_SPI is set (and using
//    a conventional Arduino SD shield), pins 50, 51, 52 and 53
//    are also available.  The RX pin from the camera (TX on
//    Arduino, second argument to SoftwareSerial()) can be any
//    pin, again excepting those used by the SD card.
// 2) You can use any of the additional three hardware UARTs on
//    the Mega board (labeled as RX1/TX1, RX2/TX2, RX3,TX3),
//    but must specifically use the two pins defined by that
//    UART; they are not configurable.  In this case, pass the
//    desired Serial object (rather than a SoftwareSerial
//    object) to the VC0706 constructor.

// Using SoftwareSerial (Arduino 1.0+) or NewSoftSerial (Arduino 0023 & prior):
#if ARDUINO >= 100
// On Uno: camera TX connected to pin 2, camera RX to pin 3:
SoftwareSerial cameraconnection = SoftwareSerial(2, 3);
// On Mega: camera TX connected to pin 69 (A15), camera RX to pin 3:
//SoftwareSerial cameraconnection = SoftwareSerial(69, 3);
#else
NewSoftSerial cameraconnection = NewSoftSerial(2, 3);
#endif

Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

// Using hardware serial on Mega: camera TX conn. to RX1,
// camera RX to TX1, no SoftwareSerial object is required:
//Adafruit_VC0706 cam = Adafruit_VC0706(&Serial1);

char Trigger = 0; //If = 1, the Arduino should be taking photos.
int32_t Photo = 0;

void setup() {

  // When using hardware SPI, the SS pin MUST be set to an
  // output (even if not connected or used).  If left as a
  // floating input w/SPI on, this can cause lockuppage.
#if !defined(SOFTWARE_SPI)
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  if(chipSelect != 53) pinMode(53, OUTPUT); // SS on Mega
#else
  if(chipSelect != 10) pinMode(10, OUTPUT); // SS on Uno, etc.
#endif
#endif

  pinMode(10, OUTPUT);
  pinMode(A1, OUTPUT); // Use thin pin as a advice LED of and incorrect setup
  digitalWrite(A1, LOW);
  pinMode(A5, OUTPUT); // Use thin pin as a advice LED of an correct ejecution of program
  digitalWrite(A5, LOW);
  pinMode(6,OUTPUT); //RST pin of the 555, to put the out of 555 to 0 when we receive a trigger from the principal uC
  digitalWrite(6, LOW);
  pinMode(5, INPUT);  //Output from the 555, when we receive first trigger we want to take photos. When we receive the second trigger, we want to stop taking photos. 
  Serial.begin(115200);
  Serial.println("VC0706 Camera snapshot test");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while(1){
      digitalWrite(A5, HIGH);
      delay(2000);
      digitalWrite(A5, LOW);
      delay(2000);
    }
  }  
  
  // Try to locate the camera
  if (cam.begin()) {
    Serial.println("Camera Found:");
  } else {
    Serial.println("No camera found?");
    while(1){
      digitalWrite(A5, HIGH);
      delay(2000);
      digitalWrite(A5, LOW);
      delay(2000);
    }
  }
  // Print out the camera version information (optional)
  /*char *reply = cam.getVersion();
  if (reply == 0) {
    Serial.print("Failed to get version");
  } else {
    Serial.println("-----------------");
    Serial.print(reply);
    Serial.println("-----------------");
  }*/

  // Set the picture size - you can choose one of 640x480, 320x240 or 160x120 
  // Remember that bigger pictures take longer to transmit!
  
  cam.setImageSize(VC0706_640x480);        // biggest
  //cam.setImageSize(VC0706_320x240);        // medium
  //cam.setImageSize(VC0706_160x120);          // small

  // You can read the size back from the camera (optional, but maybe useful?)
  uint8_t imgsize = cam.getImageSize();
  Serial.print("Image size: ");
  if (imgsize == VC0706_640x480) Serial.println("640x480");
  if (imgsize == VC0706_320x240) Serial.println("320x240");
  if (imgsize == VC0706_160x120) Serial.println("160x120");
}

void loop() {
  //Serial.println("Snap in 5 secs...");
  //delay(5000);

  if(digitalRead(5) == HIGH){ //If we read 1 at the OUT of 555, UC has sent a trigger
    Trigger ^= 1;
    digitalWrite(6, HIGH); //Activate Reset to wait for another trigger
    delay(500);
    digitalWrite(6, LOW); 
  }
  while(Trigger == 0){ //Wait until the uC tell us to take photos
    digitalWrite(A1, HIGH);
    if(digitalRead(5) == HIGH){
      delay(1000); //To wait for the 'set' pin go low
      digitalWrite(6, HIGH);
      delay(500);
      digitalWrite(6, LOW); 
      Trigger ^= 1;
    }
  }  

  digitalWrite(A5, HIGH); //Red led ON and green off to advice that the system is busy
  digitalWrite(A1, LOW);
  
  if (! cam.takePicture()) 
    Serial.println("Failed to snap!");
  else 
    Serial.println("Picture taken!");
  
  // Create an image with the name IMAGExx.JPG
  char filename[13];
  strcpy(filename, "IMAGE00.JPG");
  for (int i = 0; i < 100; i++) {
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  
  // Open the file for writing
  File imgFile = SD.open(filename, FILE_WRITE);

  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image.");

  int32_t time = millis();
  pinMode(8, OUTPUT);
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);
    if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
      Serial.print('.');
      wCount = 0;
    }
    //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");
    jpglen -= bytesToRead;
  }
  imgFile.close();

  File myFile = SD.open("Time.txt", FILE_WRITE);
  if(myFile)
  {
    myFile.println();
    myFile.print("Photo ");
    myFile.print(Photo);
    myFile.print(": ");
    myFile.print(time);
    myFile.print("ms elapsed since start");
    myFile.close();
    Photo = Photo + 1; 
  }
  else{
    Serial.println("Error opening the .txt");
  }

  time = millis() - time;
  Serial.println("done!");
  digitalWrite(A5, LOW);
  digitalWrite(A1, HIGH);
  delay(1000);
  Serial.print(time); Serial.println(" ms elapsed");
}
