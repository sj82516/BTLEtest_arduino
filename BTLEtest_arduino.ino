{\rtf1\ansi\ansicpg950\cocoartf1348\cocoasubrtf170
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
\paperw11900\paperh16840\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural

\f0\fs24 \cf0 /*********************************************************************\
This is an example for our nRF8001 Bluetooth Low Energy Breakout\
\
  Pick one up today in the adafruit shop!\
  ------> http://www.adafruit.com/products/1697\
\
Adafruit invests time and resources providing this open source code, \
please support Adafruit and open-source hardware by purchasing \
products from Adafruit!\
\
Written by Kevin Townsend/KTOWN  for Adafruit Industries.\
MIT license, check LICENSE for more information\
All text above, and the splash screen below must be included in any redistribution\
*********************************************************************/\
\
// This version uses the internal data queing so you can treat it like Serial (kinda)!\
\
#include <SPI.h>\
#include "Adafruit_BLE_UART.h"\
#include <Adafruit_NeoPixel.h>\
#ifdef __AVR__\
  #include <avr/power.h>\
#endif\
#define PIN            6\
#define NUMPIXELS      1\
// Connect CLK/MISO/MOSI to hardware SPI\
// e.g. On UNO & compatible: CLK = 13, MISO = 12, MOSI = 11\
#define ADAFRUITBLE_REQ 10\
#define ADAFRUITBLE_RDY 2     // This should be an interrupt pin, on Uno thats #2 or #3\
#define ADAFRUITBLE_RST 9\
\
Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);\
/**************************************************************************/\
/*!\
    Configure the Arduino and start advertising with the radio\
*/\
/**************************************************************************/\
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);\
\
void setup(void)\
\{ \
  Serial.begin(9600);\
  while(!Serial); // Leonardo/Micro should wait for serial init\
  Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 Print echo demo"));\
\
  BTLEserial.setDeviceName("NEWNAME"); /* 7 characters max! */\
\
  BTLEserial.begin();\
  #if defined (__AVR_ATtiny85__)\
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);\
  #endif\
  // End of trinket special code\
\
  pixels.begin(); \
\}\
\
/**************************************************************************/\
/*!\
    Constantly checks for new events on the nRF8001\
*/\
/**************************************************************************/\
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;\
\
void loop()\
\{\
  // Tell the nRF8001 to do whatever it should be working on.\
  BTLEserial.pollACI();\
  String input="";\
  // Ask what is our current status\
  aci_evt_opcode_t status = BTLEserial.getState();\
  // If the status changed....\
  if (status != laststatus) \{\
    // print it out!\
    if (status == ACI_EVT_DEVICE_STARTED) \{\
        Serial.println(F("* Advertising started"));\
    \}\
    if (status == ACI_EVT_CONNECTED) \{\
        Serial.println(F("* Connected!"));\
    \}\
    if (status == ACI_EVT_DISCONNECTED) \{\
        Serial.println(F("* Disconnected or advertising timed out"));\
    \}\
    // OK set the last status change to this one\
    laststatus = status;\
  \}\
\
  if (status == ACI_EVT_CONNECTED) \{\
    // Lets see if there's any data for us!\
    // OK while we still have something to read, get a character and print it out\
    if(BTLEserial.available())\{\
      input = "";\
      while (BTLEserial.available()) \{\
        char c = BTLEserial.read();\
        input += c;\
        Serial.print(c);\
        Serial.print(input);\
      \}\
      if(input.equals("yello"))\{\
        pixels.setPixelColor(0, pixels.Color(155,155,0));\
        pixels.show();\
      \}else if(input.equals("blue"))\{\
        pixels.setPixelColor(0, pixels.Color(0,0,255));\
  \
      \}else if(input.equals("red"))\{\
        pixels.setPixelColor(0, pixels.Color(255,0,0));\
      \}else\{\
        pixels.setPixelColor(0, pixels.Color(0,250,0));\
      \}\
   \
      pixels.show(); // This sends the updated pixel color to the hardware.\
    \}\
  \}\
\}}