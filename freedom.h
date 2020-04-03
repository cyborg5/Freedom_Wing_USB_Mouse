/*
 * Freedom Wing Library
 * by Chris Young
 */

#define FW_REF_PIN A2
#define FW_HORZ_PIN A3
#define FW_VERT_PIN A4

/* Values read from the joystick are averaged over the
   last 5 readings to dampen out noise. Increase this value
   for more dampening. Note large values will cause a delay in
   reaction speed. */
#define FW_AVG_SIZE 5

/* Theoretically if the reference value is subtracted from 
   horizontal and vertical values and the joystick is in the 
   neutral position, the end result should be zero. But 
   sometimes it's off a little bit. This fudge value helps you 
   find the proper zero point.

   The default value here is -8 however you can change it in 
   your application sketch using "fw.FudgeVal= whatever;"
 */
#define FW_FUDGE_DEFAULT -8

/* Even with filtering using a running average, you may not 
   want to actually do anything unless the joystick is beyond 
   a certain minimum value. Here is the default tolerance. You 
   can change it using "fw.Tolerance= whatever;" in your application sketch.
 */
#define FW_TOLERANCE_DEFAULT 2

class FreedomWing
{
  public:
    FreedomWing();
    bool readJoystick(int16_t &HorzVal, int16_t &VertVal);
    int8_t  FudgeVal;
    uint8_t Tolerance;
  private:
    int16_t  HorzData[FW_AVG_SIZE];
    int16_t  VertData[FW_AVG_SIZE];
    uint16_t AvgIndex;
};

FreedomWing::FreedomWing(void) {
  pinMode(FW_REF_PIN,INPUT);
  pinMode(FW_HORZ_PIN,INPUT);
  pinMode(FW_VERT_PIN,INPUT);
  //Zero out the array for averaging values.
  for (AvgIndex=0; AvgIndex<FW_AVG_SIZE; AvgIndex++) {
    HorzData[AvgIndex] = VertData[AvgIndex] = 0;
  };
  //Other initialization
  AvgIndex = 0; 
  FudgeVal = FW_FUDGE_DEFAULT;
  Tolerance = FW_TOLERANCE_DEFAULT;
};

bool FreedomWing::readJoystick (int16_t &HorzVal, int16_t &VertVal) {
  int16_t RefVal, HorzTemp, VertTemp;
  RefVal = analogRead(FW_REF_PIN)+FudgeVal; 
  //Read the horizontal and vertical values, subtract the reference
  // and store it in the array.
  HorzData[AvgIndex] = analogRead(FW_HORZ_PIN)-RefVal; 
  VertData[AvgIndex] = analogRead(FW_VERT_PIN)-RefVal; 
  //Compute the sum of the values in the array.
  HorzTemp = VertTemp = 0;
  for (int16_t i=0; i<FW_AVG_SIZE; i++) {
    HorzTemp += HorzData[i];
    VertTemp += VertData[i];
  };
  //Increment the index and wraparound to the beginning when full.
  AvgIndex=(++AvgIndex) % FW_AVG_SIZE;
  //Pass the computed averages back to the parameters call by reference.
  HorzVal = HorzTemp/FW_AVG_SIZE;
  VertVal = VertTemp/FW_AVG_SIZE; 
  //Return true if absolute value of either movement is above tolerance
  return (abs(HorzVal)>Tolerance)  || (abs(VertVal)>Tolerance);
};
