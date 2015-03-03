/**
 * @author Alexander Entinger, MSc / LXRobotics
 * @brief this module implements the library for controlling the LXRobotics Binary Watch Shield
 * @file LXR_Binary_Watch_Shield.h
 * @license Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 4.0) ( https://creativecommons.org/licenses/by-nc-sa/4.0/ )
 */

#ifndef LXR_BINARY_WATCH_SHIELD_H_
#define LXR_BINARY_WATCH_SHIELD_H_

#include <stdint.h>

class LXR_Binary_Watch_Shield
{
public:
  
  /** 
   * @brief initializes the library
   */
  static void begin();
  
  /**
   * @brief sets what number should be displayed on which digit of the binary watch shield
   */
  static void set_display(uint8_t const digit_3, uint8_t const digit_2, uint8_t const digit_1, uint8_t const digit_0);
  
private: 

  /**
   * @brief Constructor
   */
  LXR_Binary_Watch_Shield() { }

  /**
   * @brief Destructor
   */  
  ~LXR_Binary_Watch_Shield() { } 
};

#endif
