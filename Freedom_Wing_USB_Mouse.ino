/* 
 *  Demo sketch for USB mouse using Freedom Wing
 *   by Chris Young
 */

#define MY_DEBUG 0

//You should disable actual mouse movement when debugging
#define USE_MOUSE 1

#include <HID.h>
#include <Mouse.h>

#include "freedom.h"
//Create an instance of the feather wing object.
//this will also initialize input pins and clear
//the array for averaging values.

FreedomWing fw;

void setup() {
  #if (MY_DEBUG)
    Serial.begin (9600);  
    delay(2000); while (!Serial);
    Serial.println("AvgH\tAvgV");
  #endif
  #if (USE_MOUSE)
    Mouse.begin();
  #endif
}

//Speed controls the amount of mouse movement
//Directions reverse left right and up down
#define Speed 0.05
#define HorzDir 1
#define VertDir -1

void loop() {
  int16_t HorzVal, VertVal;
  //readJoystick returns 3 values. HorzVal and VertVal will contain the
  //horizontal and vertical movement of the joystick upon return.
  //The function returns true if those values were within the tolerance.
  //Any value below the tolerance we will ignore as noise around the
  //zero point.
  if (fw.readJoystick (HorzVal,VertVal)) {
    #if (USE_MOUSE)
     Mouse.move(HorzVal*HorzDir*Speed,VertVal*VertDir*Speed,0);
    #endif
  }
  #if (MY_DEBUG)
    Serial.print(HorzVal); Serial.print("\t");
    Serial.print(VertVal); Serial.print("\t");
    Serial.println();
    delay(20);
  #else
    delay(10);
  #endif
}
