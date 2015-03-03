/** 
 * @author Alexander Entinger MSc, LXRobotics
 * @brief this file demonstrates how to use the IV18 library in combination with the LXR Tubeclock shield (+ LXR_Tubeclock_Shield Library)
 * @file binary_watch_demo.ino
 * @license Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0) ( http://creativecommons.org/licenses/by-nc-sa/3.0/ )
 */

#include "LXR_Binary_Watch_Shield.h" 
#include <Wire.h> // necessary for RTClib
#include "RTClib.h"

RTC_DS1307 RTC;

void setup() 
{
  // initialize the binary watch shield
  LXR_Binary_Watch_Shield::begin();
  
  // setup the real time clock
  Wire.begin();
  RTC.begin(); 
  RTC.adjust(DateTime(__DATE__, __TIME__)); // sets the RTC to the date & time this sketch was compiled
  RTC.setSqwOutSignal(RTC_DS1307::Frequency_1Hz); // set output frequency to 1 Hz
  RTC.register_callback(&tick, EINT0);
}

static boolean update_display = false;
/**
 * @brief this function is executed once per second
 */
void tick() 
{
  // update flag
  update_display = true;
}


void loop()
{
  if(update_display)
  {
    // update flag
    update_display = false;
  
    // get current time
    DateTime now = RTC.now();
    uint8_t const hour = now.hour();
    uint8_t const minute = now.minute();
    
    // convert to digit representation
    uint8_t const dig_3 = hour / 10;
    uint8_t const dig_2 = hour % 10;
    uint8_t const dig_1 = minute / 10;
    uint8_t const dig_0 = minute % 10; 
 
    // update the display driver
    LXR_Binary_Watch_Shield::set_display(dig_3,dig_2,dig_1,dig_0);   
  }  
}
