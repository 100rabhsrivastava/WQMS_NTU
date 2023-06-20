
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
//int i ;
#define WHITE 0xFFFF
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define SD_CS 4
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC , TFT_RST);
File myFile;
String message = "";
bool messageReady = false;
int j =-1 ;
#define PIN_A10 ;
#define PIN_A11 ;
#define PIN_A12 ;
#define PIN_A13 ;
#define PIN_A14 ;
#define PIN_A5 ; // voltage sensor 1 power wali  pcb 
#define PIN_A5 ; // VS2  sensor ke pass ka voltage 
#define PIN_A6 ; // 
long int loop_flag=0;
const int buttonPin = 2;  // Digital pin connected to the push button
int buttonState = HIGH;   // Current state of the button
int lastButtonState = HIGH;  // Previous state of the button

void setup() {
   pinMode(5, OUTPUT);
   pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 
  tft.begin();
}

void loop ()
  {

    tft.setRotation(1);
    if(loop_flag==0){
     welcome();
     wificheckprint();
      sdcardinfo();
    }
    if(loop_flag%15147480==0){
      j = sdcardinit();
     
      loop_flag = 0; 
    }
     if(loop_flag==0){
     voltagecheckpoint();
    }
    if(loop_flag==0){
     sensorconnectivity();
    }
    if(loop_flag==0){
     sensorstab();
    }
    if(loop_flag==0){
     webdisplay();
    }
    datacomm();
   if (j==1 && loop_flag%500000==0 ){ sdcardwrite();}
    loop_flag++;

       buttonState = digitalRead(buttonPin);
       if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      Serial.println("Button Pressed");
      sdcardread();
    } else {
      Serial.println("Button Released");
    }
    lastButtonState = buttonState;  // Update the last button state
  }
  }

unsigned long welcome() {
  tft.fillScreen(ILI9341_RED);
  tft.fillScreen(ILI9341_GREEN);
  tft.fillScreen(ILI9341_YELLOW);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(85, 30);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(3);
  tft.println("Welcome!!");
  tft.setCursor(150, 75);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  tft.println("To");
  tft.setCursor(50, 110);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(3);
  tft.println("Water Quality ");
  tft.setCursor(10, 150);
 //  tft.setFont(&FreeSans9pt7b);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(3);
  tft.println("Monitoring System");
  tft.setCursor(100, 190);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(5);
  tft.println("#WQMS");
  delay(9000);
  
}
unsigned long wificheckprint(){
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 30);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("Please Check The WiFi LED");
  tft.setCursor(15, 68);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("LED ON  : Connected ");
  tft.setCursor(15, 99);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("LED OFF : Not Connected  ");
  tft.setCursor(50, 140);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
  tft.println("Press RESET switch ,");
  tft.setCursor(55, 160);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
  tft.println("If Not Connected... ");
  delay(7000);
  }

//unsigned long insertsdcard(){
//  tft.setCursor(45, 30);
//  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
//  tft.println("Please Insert ");
//  tft.setCursor(45, 60);
//  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
//  tft.println("the SD Card");
//  tft.setCursor(45, 90);
//  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
//  tft.println("and wait for");
//  tft.setCursor(45, 120);
//  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
//  tft.println("initialization");
//  delay(8000);
//
//  }

void datacomm() {
    digitalWrite(5, HIGH);
  // Monitor serial communication
  while(Serial.available()) {
    message = Serial.readString();
    messageReady = true;
  }
  // Only process message if there's one
  if(messageReady) {
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(1024); // ArduinoJson version 6+
    // Attempt to deserialize the message
    DeserializationError error = deserializeJson(doc,message);
    if(error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }
    if(doc["type"] == "request") {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["Asensor0"] = analogRead(A10);
      doc["Asensor1"] = analogRead(A11);
      doc["Asensor2"] = analogRead(A12);
      doc["Asensor3"] = analogRead(A13);
      doc["Asensor4"] = analogRead(A14);
      serializeJson(doc,Serial);
    }
    messageReady = false;
  }
}
int sdcardinit()
 {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(45, 30);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("Please Insert ");
  tft.setCursor(45, 60);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("the SD Card");
  tft.setCursor(45, 90);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("and wait for");
  tft.setCursor(45, 120);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("initialization");
  delay(8000);
   tft.fillScreen(ILI9341_BLACK); 
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed!");
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(20, 30);
    tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
    tft.println("Initialization Failed.");
    tft.setCursor(15, 68);
    tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
    tft.println("Things to check:");
    tft.setCursor(15, 99);
    tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
    tft.println("* is a card inserted? ");
    tft.setCursor(15, 125);
    tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
    tft.println("* is your wiring correct?");
    tft.setCursor(15, 150);
    tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
    tft.println("* did you change the");
    tft.setCursor(15, 173);
    tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
    tft.println("  chip Select pin to");
    tft.setCursor(15, 194);
    tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
    tft.println("  match your shield");
    delay(7000);
   return 0;
  }
  Serial.println("initialization done.");
  tft.fillScreen(ILI9341_BLACK);
  tft.println(" ");
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.setCursor(20, 30);
  tft.println("Initialization Done");
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(20, 68);
  tft.println("* Wiring is correct and");
  tft.setCursor(20, 87);
  tft.println("  a card is present. ");
  delay(2000);
  return 1;
  }

void sdcardinfo(){
  Sd2Card card;
    Serial.print("Card type:         ");
   {
    Serial.print(card.type());
  switch (card.type()) {
     Serial.println(card.type);
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
    }

} 
void sdcardwrite()
{
   myFile = SD.open("DHT11Log.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(analogRead(A10));
    myFile.print(',');
    myFile.print(analogRead(A11));
    myFile.print(',');
    myFile.print(analogRead(A12));
    myFile.print(',');
    myFile.print(analogRead(A13));
    myFile.print(',');
    myFile.println(analogRead(A14));
    myFile.close();
   // Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening DHT11Log.txt");
  }
}
void sdcardread(){
myFile = SD.open("DHT11Log.txt");
  if (myFile) {
    while (myFile.available()) {
      String line = myFile.readStringUntil('\n');
      tft.setTextSize(2);
     tft.setTextColor(ILI9341_CYAN);
      tft.print(line);
      Serial.println(line);
    }
    myFile.close();
  } else {
    Serial.println("File not found.");
    tft.setTextSize(2);
      tft.setTextColor(ILI9341_CYAN);
      tft.print("File not found.");
  }
  delay(5000);
  
}

unsigned long sensorconnectivity(){
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 30);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("Connect All The Sensors");
  tft.setCursor(10, 68);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("LED ON  : All Sensors  ");
  tft.setCursor(10, 88);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("          Connected ");
  tft.setCursor(10, 120);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("LED OFF : Kindly Connect");
  tft.setCursor(10, 140);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("          The Sensors ");
  tft.setCursor(10, 159);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("          Properly");
  delay(9000);
  }

unsigned long sensorstab(){
  int count = 15; 
      for (int i = count; i >= 0; i--) 
      {
       tft.fillScreen(ILI9341_BLACK);
       tft.setCursor(15, 30);
       tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
       tft.println("Stabilizing The Sensors ");
       tft.setCursor(10, 68);
       tft.setCursor(120, 100);
       tft.setTextColor(ILI9341_RED);  tft.setTextSize(11);
       tft.println(i);
       delay(1000);
        
       }
  }


 void voltagecheckpoint()
 { 
  int offset =40;
  int volt1 = analogRead(A4);// read the input
  int volt2 = analogRead(A5);// read the input
  int volt3 = analogRead(A6);// read the input
  double voltage1 = map(volt1,0,1023, 0, 2500) - offset;
  double voltage2 = map(volt2,0,1023, 0, 2500) - offset;
  double voltage3 = map(volt3,0,1023, 0, 2500) - offset;
  voltage1 /=100;// 
  voltage2 /=100;// 
  voltage3 /=100;// 
  Serial.print("Voltage Reading: ");
  Serial.print(voltage1);//print the voltge
  Serial.println("V");
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 30);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("CheckPoints  ");
  tft.setCursor(10, 68);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("A :  ");
  tft.setCursor(30, 68);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println(voltage1);
  tft.setCursor(10, 118);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("B :  ");
  tft.setCursor(30, 118);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println(voltage1);

  if(voltage1 || voltage2 < 12)
  {
  tft.setCursor(70, 68);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
  tft.println("Error");
  tft.setCursor(70, 118);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
  tft.println("Error");
  tft.setCursor(20, 148);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
  tft.println("Voltage Error");
  tft.setCursor(20, 148);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
  tft.println("Voltage Error");
  tft.setCursor(20, 178);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(2);
  tft.println("Auto Power Off in 7 sec");

  }
  else{
  tft.setCursor(70, 68);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("V");
  tft.setCursor(70, 118);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("V");
  }
  delay(9000);
 }

 unsigned long webdisplay(){
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 30);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.println("Sending Data On :");
  tft.setCursor(10, 68);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("192.168.0.142");
  tft.setCursor(10, 120);
  tft.setTextColor(ILI9341_GREEN);  tft.setTextSize(2);
  tft.println("www.wqms.ml");
  delay(5000);
  }
