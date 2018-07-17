/*ESP8266WiFiWebServer
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.
*/

/*
Original project found here https://github.com/giznoise/Arduino-ESP8266-RGB-Web-Server
*/

//Public Release Version


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#define FASTLED_ESP8266_RAW_PIN_ORDER //Refer to FastLED notes on ESP8266 for an explanation on these modes
//This define must be called before including FastLED
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "html_pages.h"
#include <FS.h>

//FastLED setup
#define LED_PIN     14
#define NUM_LEDS    13
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 50
static uint8_t startIndex = 0;
//End FastLED defines

//MQTT Connection
#define mqtt_server "io.adafruit.com"
#define mqtt_user "Adafruit-Username"
#define mqtt_password "Adafrui API-Key"
WiFiClient espClient;
PubSubClient client(espClient);
#define control_topic "Adafruit-Username/feeds/FeedName"

//Wifi settings
const char *ssid = "Your SSID";
const char *password = "Wifi Password";

MDNSResponder mdns;
ESP8266WebServer server ( 80 );

//FastLED stuff
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

CRGB leds[NUM_LEDS];

const int delayval = 500;
String rgb_now = "#0000ff";    //global rgb state values for use in various html pages
long red_int = 0;
long green_int = 0;
long blue_int = 255;
int brightness = 255;
int mode_flag = 1;



void handleIndex();
void handleNotFound();
void handleSwitchOn();
void handleSwitchOff();
void handleSetColour();
void handleColour();
void handleSetBrightness();
void handleBrightness();
void handleSelectMode();
void handleSelectPalette(); //Select palette
void handle_mode1();
void handle_mode2();
void handle_mode3();
void handle_mode4();
void handle_mode5(); //Palette selection mode
void handle_mode6();
void light_up_all();
void turn_off_all();
//Palette select modes
void handle_cloud();
uint32_t Wheel(byte);

//Serving JS library from SPIFFS
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)

void setup ( void ) {
  Serial.begin ( 115200 );
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setDither(1); //Allows for low brightness not to flicker
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, 900);
  setupWiFi();

  Serial.println ( "" );

  EEPROM.begin(5);                           // set up EEPROM storage space for config values

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

//MQTT Setup
  client.setServer(mqtt_server, 1883);
  client.subscribe(control_topic);
  Serial.print("Subsribed to topic");
  client.setCallback(callback);
  Serial.print("Callback set");

//Webpage handling setup
  Serial.println ( "HTTP server started" );

  server.on ( "/", handleIndex );
  server.onNotFound ( handleNotFound );

  server.on ( "/switch_on", handleSwitchOn);
  server.on ( "/switch_off", handleSwitchOff);
  server.on ( "/set_colour", handleSetColour);
  server.on ( "/set_colour_hash", handleColour );
  server.on ( "/set_brightness", handleSetBrightness);
  server.on ( "/set_bright_val", handleBrightness);
  server.on ( "/select_mode", handleSelectMode);
  server.on ( "/select_palette", handleSelectPalette);
  server.on ( "/set_mode1", handle_mode1);
  server.on ( "/set_mode2", handle_mode2);
  server.on ( "/set_mode3", handle_mode3);
  server.on ( "/set_mode4", handle_mode4);
  server.on ( "/set_mode5", handle_mode5);
  server.on ( "/set_mode6", handle_mode6);
  server.on ( "/set_cloud", handle_cloud);
  server.on ( "/set_party", handle_party);
  server.on ( "/set_heat",  handle_heat);
  server.on ( "/set_ocean", handle_ocean);


  //EEPROM Memory//
  //Mem Location ||--0--||--1--||--2--||--3--||--4--||--5--||--6--||
  //                red   green  blue   bright  mode

  red_int = EEPROM.read(0);       //restore colour to last used value. Ensures RGB lamp is same colour as when last switched off
  green_int = EEPROM.read(1);
  blue_int = EEPROM.read(2);
  brightness = EEPROM.read(3);
  mode_flag = EEPROM.read(4);

  SPIFFS.begin();
  server.begin();

  handleSwitchOn();
}

//MQTT Callback
void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String ON = String("ON");
  String OFF = String("OFF");
  payload[length] = '\0';
  String value = String((char*)payload);
  if (value == ON) {
    Serial.print("ON Received!");
    handleSwitchOn();
   }
  if (value == OFF) {
    Serial.print("OFF Received!");
    handleSwitchOff();
  }
  
  Serial.println (value);
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    Serial.write((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
        // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(control_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setupWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  //OTA update setup
  ArduinoOTA.setHostname("Lightbox-ESP8266");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  //End OTA
  //Setting up MDNS service
  delay(1000);
  if (!MDNS.begin("lightbox")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".js")) return "application/javascript";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  //if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

void loop ( void ) {
  mdns.update();
  ArduinoOTA.handle();
  //MQTT Handling
  server.handleClient();
    if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //End MQTT handling
  
  //FastLED.setBrightness(brightness);
  //currentPalette = RainbowColors_p;
  //currentBlending = LINEARBLEND;
  //startIndex = 0;
  //ChangePalettePeriodically();
  //uint8_t startIndex = 0;
  //startIndex = startIndex + 1; /* motion speed */
  //FillLEDsFromPaletteColors( startIndex);
  //FastLED.show();
  //FastLED.delay(1000 / UPDATES_PER_SECOND);

  if (mode_flag == 1) {
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }

  if (mode_flag == 2 || mode_flag == 3) {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    FillLEDsFromPaletteColors( startIndex);

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }

  if (mode_flag == 4) {
    ChangePalettePeriodically();
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    FillLEDsFromPaletteColors( startIndex);

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }

  if (mode_flag == 5 || mode_flag == 6) {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    FillLEDsFromPaletteColors( startIndex);

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }

}

void handleIndex() {
  Serial.println ( "Request for index page received" );
  server.send ( 200, "text/html", page_contents);
}

void handleNotFound() {
  if (!handleFileRead(server.uri())) {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}
}

void handleSwitchOn() {
  mode_flag = EEPROM.read(4);                       // start-up in last saved mode
  delay(100);
  switch (mode_flag) {
    case 1: handle_mode1();
      break;
    case 2: handle_mode2();
      break;
    case 3: handle_mode3();
      break;
    case 4: handle_mode4();
      break;
    case 5: handle_mode5();
      break;
    case 6: handle_mode6();
      break;
    case 7: handle_cloud();
    default:
      light_up_all();                          //Default to fixed colour should the EEProm become corrupted
      break;
  }
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>" );
};

void handleSwitchOff() {
  mode_flag = 1;                                     //go to default fixed color mode and turn off all pixels
  delay(100);
  turn_off_all();
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>" );
}

void handleSetColour() {
  server.send ( 200, "text/html", colour_picker);
}


void handleSetBrightness() {
  server.send ( 200, "text/html", bright_set);
}


void handleSelectMode() {
  server.send ( 200, "text/html", mode_page );
  //  Serial.println ( "Mode select page" );
}

void handleSelectPalette() {
  server.send ( 200, "text/html", palette_page );
  //  Serial.println ( "Palette select page" );
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  //uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}
void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
    if ( secondHand == 10)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);

    }
    if ( secondHand == 15)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
    if ( secondHand == 20)  {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
    if ( secondHand == 25)  {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
    if ( secondHand == 30)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = NOBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
    if ( secondHand == 35)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
    if ( secondHand == 40)  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
    if ( secondHand == 45)  {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
    if ( secondHand == 50)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = NOBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
    if ( secondHand == 55)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = LINEARBLEND;
      Serial.println("Second hand: ");
      Serial.println(secondHand);
    }
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};
void handleColour() {
  char buf_red[3];                               //char buffers to hold 'String' value converted to char array
  char buf_green[3];
  char buf_blue[3];
  String message = server.arg(0);                //get the 1st argument from the url which is the hex rgb value from the colour picker ie. #rrggbb (actually %23rrggbb)
  rgb_now = message;
  rgb_now.replace("%23", "#");                   // change %23 to # as we need this in one of html pages
  Serial.println(rgb_now);
  //String red_val = rgb_now.substring(1, 3);      //extract the rgb values
  //String green_val = rgb_now.substring(3, 5);
  //String blue_val = rgb_now.substring(5, 7);
  String red_val = rgb_now.substring(0, 2);      //extract the rgb values
  String green_val = rgb_now.substring(2, 4);
  String blue_val = rgb_now.substring(4, 6);     //Offsetting array. JSCOLOR does not send the # with the data
  Serial.println(red_val);
  Serial.println(green_val);
  Serial.println(blue_val);
  mode_flag = 1;                                 //get to fixed colour mode if not already

  red_val.toCharArray(buf_red, 3);               //convert hex 'String'  to Char[] for use in strtol()
  green_val.toCharArray(buf_green, 3);
  blue_val.toCharArray(buf_blue, 3);

  red_int = gamma_adjust[strtol( buf_red, NULL, 16)];          //convert hex chars to ints and apply gamma adjust
  green_int = gamma_adjust[strtol( buf_green, NULL, 16)];
  blue_int = gamma_adjust[strtol( buf_blue, NULL, 16)];

  EEPROM.write(0, red_int);                 //write the colour values to EEPROM to be restored on start-up
  EEPROM.write(1, green_int);
  EEPROM.write(2, blue_int);
  //trying to fix on and off
  EEPROM.write(4, mode_flag);                         //write mode to EEProm so can be restored on start-up
  //end test
  EEPROM.commit();

  light_up_all();
  String java_redirect = "<SCRIPT language='JavaScript'>window.location='/set_colour?";
  java_redirect += message;                                                 //send hash colour value in URL to update the colour picker control
  java_redirect += "';</SCRIPT>";
  server.send ( 200, "text/html", java_redirect );                                 // all done! - take user back to the colour picking page
}

void handleBrightness() {
  String message = server.arg(0);                //get the 1st argument from the url which is the brightness level set by the slider
  String bright_val = message.substring(0, 3);   //extract the brightness value from the end of the argument in the URL
  brightness =  bright_val.toInt();
  FastLED.setBrightness(brightness);
  EEPROM.write(3, brightness);                   //write the brightness value to EEPROM to be restored on start-up
  EEPROM.commit();

  String java_redirect = "<SCRIPT language='JavaScript'>window.location='/set_brightness?";
  java_redirect += brightness;                                              //send brightness value in URL to update the slider control
  java_redirect += "';</SCRIPT>";
  server.send ( 200, "text/html", java_redirect);                                  // all done! - take user back to the brightness selection page
}

void light_up_all() {
  //FastLED
  fill_solid(leds, NUM_LEDS, CRGB(brightness * red_int / 255, brightness * green_int / 255, brightness * blue_int / 255));
  FastLED.show();
}

void turn_off_all() {
  mode_flag = 999;                                     //go to non-existent mode and turn off all pixels
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  FastLED.show();

}

void handle_mode1() {                                 //fixed colour mode
  mode_flag = 1;
  EEPROM.write(4, mode_flag);                         //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");

  while (mode_flag == 1) {                            // Check the mode hasn't been changed whilst we wait, if so - leave immediately
    light_up_all();                           //set mode to default state - all led's on, fixed colour. This loop will service any brightness changes
    loop();                                   // Not much to do except service the main loop
  } return;
}


void handle_mode2() {                                //colour fade mode
  mode_flag = 2;
  EEPROM.write(4, mode_flag);                        //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");
  uint16_t i, j, k;
  int wait = 10;  //DON'T ever set this more than '10'. Use the 'k' value in the loop below to increase delays. This prevents the watchdog timer timing out on the ESP8266
  //Serial.println("On Mode 2 - Color Fade");
  while (mode_flag == 2) {
    for (j = 0; j < 256; j++) {
      loop();
      for (i = 0; i < NUM_LEDS; i++) {
        loop();
        //startIndex = 0;
        //startIndex = startIndex + 1;
        currentPalette = PartyColors_p;
        currentBlending = LINEARBLEND;
        //FillLEDsFromPaletteColors(startIndex);
        //FastLED.show();
      } loop();
      //startIndex = 0;
      for (k = 0; k < 200; k++) {                      // Do ten loops of the 'wait' and service loop routine inbetween. Total wait = 10 x 'wait'. This prevents sluggishness in the browser html front end menu.
        delay(wait);
        loop();
      } loop();
    }
    return;
  }
}

void handle_mode3() {                               //rainbow mode
  mode_flag = 3;
  EEPROM.write(4, mode_flag);                       //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");
  uint16_t i, j, k;
  int wait = 10;  //DON'T ever set this more than '10'. Use the 'k' value in the loop below to increase delays. This prevents the watchdog timer timing out on the ESP8266
  //Serial.println("On Mode 3 - Rainbow");
  while (mode_flag == 3) {                           // do this indefenitely or until mode changes
    for (j = 0; j < 256 * 5; j++) {                   // 5 cycles of all colors on wheel
      loop();
      for (i = 0; i < NUM_LEDS; i++) {
        loop();
        currentPalette = RainbowColors_p;
        currentBlending = LINEARBLEND;
        //FillLEDsFromPaletteColors(startIndex);
        //FastLED.show();
      }
        //FastLED.show();

      for (k = 0; k < 50; k++) {                     // Do ten loops of the 'wait' and service loop routine inbetween. Total wait = 10 x 'wait'. This prevents sluggishness in the browser html front end menu.
        delay(wait);
        loop();
      }
    }
  } return;
}



void handle_mode4() {                               //Random mode
  mode_flag = 4;
  EEPROM.write(4, mode_flag);                       //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  //Serial.println("On Mode 4 - Random");
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");
  uint16_t i, j, k;
  int wait = 10;  //DON'T ever set this more than '10'. Use the 'k' value in the loop below to increase delays. This prevents the watchdog timer timing out on the ESP8266
  //mode_flag = 4;
  while (mode_flag == 4) {                           // do this indefenitely or until mode changes
    for (j = 0; j < 256; j++) {                   // 5 cycles of all colors on wheel
      loop();
      for (i = 0; i < NUM_LEDS; i++) {
        loop();
        ChangePalettePeriodically();
      }
       //FastLED.show();

      for (k = 0; k < 50; k++) {                     // Do ten loops of the 'wait' and service loop routine inbetween. Total wait = 10 x 'wait'. This prevents sluggishness in the browser html front end menu.
        delay(wait);
        loop();
      }
    }
  } return;

}

void handle_mode5() {                               //Palette Select, Linearblend mode
  mode_flag = 5;
  EEPROM.write(4, mode_flag);                       //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  //Serial.println("On Mode 5 - Palette Picker");
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");
  uint16_t i, j, k;
  int wait = 10;  //DON'T ever set this more than '10'. Use the 'k' value in the loop below to increase delays. This prevents the watchdog timer timing out on the ESP8266
  //mode_flag = 4;
  while (mode_flag == 5) {                           // do this indefenitely or until mode changes
    for (j = 0; j < 256; j++) {                   // 5 cycles of all colors on wheel
      loop();
      for (i = 0; i < NUM_LEDS; i++) {
        loop();
        currentBlending = LINEARBLEND;
      }
      //FastLED.show();

      for (k = 0; k < 50; k++) {                     // Do ten loops of the 'wait' and service loop routine inbetween. Total wait = 10 x 'wait'. This prevents sluggishness in the browser html front end menu.
        delay(wait);
        loop();
      }
    }
  } return;

}

void handle_mode6() {                               //Palette Select, Noblending mode
  mode_flag = 6;
  EEPROM.write(4, mode_flag);                       //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  //Serial.println("On Mode 6 - Palette Picker");
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");
  uint16_t i, j, k;
  int wait = 10;  //DON'T ever set this more than '10'. Use the 'k' value in the loop below to increase delays. This prevents the watchdog timer timing out on the ESP8266
  //mode_flag = 4;
  while (mode_flag == 6) {                           // do this indefenitely or until mode changes
    for (j = 0; j < 256; j++) {                   // 5 cycles of all colors on wheel
      loop();
      for (i = 0; i < NUM_LEDS; i++) {
        loop();
        currentBlending = NOBLEND;
      }
      //FastLED.show();

      for (k = 0; k < 50; k++) {                     // Do ten loops of the 'wait' and service loop routine inbetween. Total wait = 10 x 'wait'. This prevents sluggishness in the browser html front end menu.
        delay(wait);
        loop();
      }
    }
  } return;

}

void handle_cloud() {
  mode_flag = 5;
  EEPROM.write(4, mode_flag);                       //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");
  uint16_t i, j, k;
  int wait = 10;  //DON'T ever set this more than '10'. Use the 'k' value in the loop below to increase delays. This prevents the watchdog timer timing out on the ESP8266
  //mode_flag = 4;
  while (mode_flag == 5) {                           // do this indefenitely or until mode changes
    for (j = 0; j < 256; j++) {                   // 5 cycles of all colors on wheel
      loop();
      for (i = 0; i < NUM_LEDS; i++) {
        loop();
        currentPalette = CloudColors_p;
      }
      //FastLED.show();

      for (k = 0; k < 50; k++) {                     // Do ten loops of the 'wait' and service loop routine inbetween. Total wait = 10 x 'wait'. This prevents sluggishness in the browser html front end menu.
        delay(wait);
        loop();
      }
    }
  } return;
}

void handle_party() {
  mode_flag = 5;
  EEPROM.write(4, mode_flag);                       //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");
  uint16_t i, j, k;
  int wait = 10;  //DON'T ever set this more than '10'. Use the 'k' value in the loop below to increase delays. This prevents the watchdog timer timing out on the ESP8266
  //mode_flag = 4;
  while (mode_flag == 5) {                           // do this indefenitely or until mode changes
    for (j = 0; j < 256; j++) {                   // 5 cycles of all colors on wheel
      loop();
      for (i = 0; i < NUM_LEDS; i++) {
        loop();
        currentPalette = PartyColors_p;
      }
      //FastLED.show();

      for (k = 0; k < 50; k++) {                     // Do ten loops of the 'wait' and service loop routine inbetween. Total wait = 10 x 'wait'. This prevents sluggishness in the browser html front end menu.
        delay(wait);
        loop();
      }
    }
  } return;
}

void handle_heat() {
  mode_flag = 5;
  EEPROM.write(4, mode_flag);                       //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");
  uint16_t i, j, k;
  int wait = 10;  //DON'T ever set this more than '10'. Use the 'k' value in the loop below to increase delays. This prevents the watchdog timer timing out on the ESP8266
  //mode_flag = 4;
  while (mode_flag == 5) {                           // do this indefenitely or until mode changes
    for (j = 0; j < 256; j++) {                   // 5 cycles of all colors on wheel
      loop();
      for (i = 0; i < NUM_LEDS; i++) {
        loop();
        currentPalette = HeatColors_p;
      }
      //FastLED.show();

      for (k = 0; k < 50; k++) {                     // Do ten loops of the 'wait' and service loop routine inbetween. Total wait = 10 x 'wait'. This prevents sluggishness in the browser html front end menu.
        delay(wait);
        loop();
      }
    }
  } return;
}

void handle_ocean() {
  mode_flag = 5;
  EEPROM.write(4, mode_flag);                       //write mode to EEProm so can be restored on start-up
  EEPROM.commit();
  server.send ( 200, "text/html", "<SCRIPT language='JavaScript'>window.location='/';</SCRIPT>");
  uint16_t i, j, k;
  int wait = 10;  //DON'T ever set this more than '10'. Use the 'k' value in the loop below to increase delays. This prevents the watchdog timer timing out on the ESP8266
  //mode_flag = 4;
  while (mode_flag == 5) {                           // do this indefenitely or until mode changes
    for (j = 0; j < 256; j++) {                   // 5 cycles of all colors on wheel
      loop();
      for (i = 0; i < NUM_LEDS; i++) {
        loop();
        currentPalette = OceanColors_p;
      }
      //FastLED.show();

      for (k = 0; k < 50; k++) {                     // Do ten loops of the 'wait' and service loop routine inbetween. Total wait = 10 x 'wait'. This prevents sluggishness in the browser html front end menu.
        delay(wait);
        loop();
      }
    }
  } return;
}
