/*
 * Project: Office Network Board (ONB)
 * Description: An interactable message board to update a message of your choice!
 * Author: Georgia Snelling, Paul Kollat, and Jack Raney
 * Date: April 2019

 * Hookup:       Screen:          PHOTON:
 *  3V3 (red) ---> 3V3       ---> 3V3
 *  GND (blk) ---- GND       ---- GND
 *  DIN (blu) ---> MOSI(D11) ---> MOSI(A5)    MOSI = Master Out Slave In
 *  CLK (ylo) ---> SCLK(D13) ---> SCK (A3)
 *  CS  (ora) ---> SS  (D10) ---> SS  (A2)
 *  DC  (grn) ---> OUT (D9)  ---> OUT (A1)
 *  RST (wht) ---> OUT (D8)  ---> OUT (A0)
 *  BUSY(pur) <--- IN  (D7)  <--- IN  (D4)
 */
 

//Libraries to be included
#include <Adafruit_GFX_RK.h>
#include <GxEPD2_PP.h>

#define ENABLE_GxEPD2_GFX 0

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <FreeMonoBold9pt7b.h>

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>


//Defining the I/O Pins
#define CS_PIN   A2
#define DC_PIN   A1
#define RST_PIN  A0
#define BUSY_PIN  A7


//4.2in Waveshare 3 Color Screen
GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT> display(GxEPD2_420c(CS_PIN, DC_PIN, RST_PIN, BUSY_PIN));


//Function Initialize Screen
//Set up all variables for screen and text settings
void initializeScreen() {
  // Clear the screen and initialize it with the template for the message
  display.init();
  display.setFont(&FreeMonoBold9pt7b);                  //Set font
  display.setTextColor(GxEPD_BLACK);                    //Set text colour as black.  Can change to be black, white, or yellow if desired
  display.setTextSize(1);                               //Setting text size to 1.  Change as needed
  display.setRotation(0);                               //Change to 1 for portrait view, 0 for landscape
  display.fillScreen(GxEPD_BLACK);                      //Setting background colour to be white.  Can be black or yellow if desired
}


//Setup Function
//Defines the function 'Update Message To Screen'
void setup() {
  Serial.begin(9600);
  initializeScreen();

  //Defining the particle Function to be used via the console or app
  bool function = Particle.function("MessageUpdate", updateMessageToScreen);

  Serial.println("setup ready");
}

//Main Loop
void loop() {
  //Deep sleep mode
   if (Time.hour()==20) {
       System.sleep(SLEEP_MODE_DEEP,15*3600);
   }
}

//Function processNewMessage
//Updates the screen with the nex message
//Input Parameters: String newMessage is the new message to be updated on the screen
void processNewMessage(String newMessage){
  
  const String text = newMessage;
  int16_t tbx, tby; uint16_t tbw, tbh;                   // boundary box window
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = (display.width() - tbw) / 2;
  uint16_t y = (display.height() + tbh) / 2; 
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.setCursor(x, y);                            // set the postition to start printing text
    display.print(text);                                // print some text
    // end of part executed multiple times
  }
  while (display.nextPage());
  Serial.println("done");
  
}


//Function connected to Device
//Input parameters: String updatedMessage is the new message to be updated on the screen
//Returns 0
int updateMessageToScreen(String updatedMessage)
{
  String newMessage = updatedMessage;
  
  if (newMessage == "" || newMessage == "clear")
  {
    newMessage = "Dr. Estell's Office Board"; //default message, change as you please (:
  } 
    
  Serial.println(newMessage);
  processNewMessage(newMessage);
  return 0;
}

