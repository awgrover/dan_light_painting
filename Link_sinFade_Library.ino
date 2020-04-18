// Library of sinFade functions
// Items in the commented heading below must be included in the linked sketch.
/*
  #include "debug.h"

  const int BRIGHTNESS = A0;      // e/g. A0. Value pot pin number; use -1 for no pot
  const float BRIGHTNESS_OVERRIDE = 0.2; // e.g. A0. if pin == -1, use this as brightness
  const int SLOWNESS = A1;        // e.g. A1. Pot pin number; 0 is fast, 1023 is slow.
  const float SLOWNESS_OVERRIDE = 1; // if pin == -1, use this as slowness

  // Which LED interface?
  #include "PWM_TLC59711.h"
  PWM_TLC59711 PWM;
  // OR
  #include "PWM_NativePin.h"
  PWM_NativePin PWM;

  // Pins for each zone, using logical pins from the PWM set (see .h above)
  const byte aLunitPins[3] = {PWM.pwm12, PWM.pwm11, PWM.pwm10};    // Zone A    >> Zone 1   {R, G, B}
  const byte bLunitPins[3] = {PWM.pwm1, PWM.pwm2, PWM.pwm3};       // Zone B    >> Zone 2   {R, G, B}
  const byte cLunitPins[3] = {PWM.pwm7, PWM.pwm8, PWM.pwm9};      // Zone C    >> Zone 3   {R, G, B}
  const byte dLunitPins[3] = {PWM.pwm6, PWM.pwm5, PWM.pwm4};       // Zone D    >> Zone 4   {R, G, B}
  const byte *LunitZones[3] = {aLunitPins, bLunitPins, cLunitPins}; // <<< fadeZone = 0, 1, 2.

  // Percent scaling for R vs G vs B for "White Balance" empirical values for LED strips.
  const float GrnScaler = 77.0;   // <<Wht balance Entrance.  << 9/2018 - 80% 75%
  const float BluScaler = 57.0;   // <<Wht balance Entrance.  << 9/2018 - 65% 55%
*////


//////////////////////////////////////////
void HSV_sinFade_Retain_1Z (float Steps, int fadeZone, float startHSV[], float HueDegrees, int newSat, int newVal) {
  //  HueDegrees: (-180 to 180). newSat and newVal:(1 to 100).  fadeZone: a=0,b=1,c=2.
  if (HueDegrees > 180) {            // Limit "HueDegrees" to -180 to 180.
    HueDegrees = HueDegrees - 360;
  }
  if (HueDegrees < -180) {
    HueDegrees = HueDegrees + 360;
  }
  float localHSV[3];
  float ValKnob;    //  << Variable for Brightness knob.

  for (int Stepr = 0; Stepr <= Steps; Stepr++) {    ///////////// <<<<<<<<<<<<  START FADE

    ValKnob = brightness();
    //<< Knob reads value 0 to 1023.
    localHSV[0] = startHSV[0] + (1 - cos(radians(Stepr * 180.0 / Steps))) * HueDegrees / 2 ;
    localHSV[1] = startHSV[1] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newSat - startHSV[1]) / 2 ;
    // value, so brightness goes here (fixme: move to the PWM class)
    localHSV[2] = (startHSV[2] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newVal - startHSV[2]) / 2 ) * ValKnob  ;

    HSVcheckLimits(localHSV);
    HSVwriteToLEDs(localHSV, fadeZone);  // << Includes whiteBalance.
    delay( delayKnob(4.4) );           // This delay determines the speed of the fade.
  }                                    ///////////////// <<<<<< END FADE process  END FADE.
}

/////////////////////////////////////////////
void HSV_sinFade_Replace_1Z (float Steps, int fadeZone, float startHSV[], float HueDegrees, int newSat, int newVal) {
  HSV_sinFade_Retain_1Z (Steps, fadeZone, startHSV, HueDegrees, newSat, newVal);
  // Set final values for the fade to pass forward for next fade.
  startHSV[0] = startHSV[0] + HueDegrees;  // newHSV1[3] is the final color of the fade.
  startHSV[1] = newSat;
  startHSV[2] = newVal;

  HSVcheckLimits(startHSV);

}
//////////
//////////////////////////////////////////
void HSV_sinFade_Retain_2Z(float Steps,
                           int fadeZone1, float startHSV1[], float HueDegrees1, int newSat1, int newVal1,
                           int fadeZone2, float startHSV2[], float HueDegrees2, int newSat2, int newVal2 ) {
  //  HueDegrees: (-180 to 180). newSat and newVal:(1 to 100).  fadeZone: a=0,b=1,c=2.
  if (HueDegrees1 > 180) {            // Limit "HueDegrees" to -180 to 180.
    HueDegrees1 = HueDegrees1 - 360;
  }
  if (HueDegrees1 < -180) {
    HueDegrees1 = HueDegrees1 + 360;
  }
  if (HueDegrees2 > 180) {            // Limit "HueDegrees" to -180 to 180.
    HueDegrees2 = HueDegrees2 - 360;
  }
  if (HueDegrees2 < -180) {
    HueDegrees2 = HueDegrees2 + 360;
  }
  float localHSV1[3];
  float localHSV2[3];
  float ValKnob;                   //  << Variable for Brightness knob.

  for (int Stepr = 0; Stepr <= Steps; Stepr++) {    ///////////// <<<<<<<<<<<<  START FADE
    ValKnob = brightness();

    localHSV1[0] = startHSV1[0] + (1 - cos(radians(Stepr * 180.0 / Steps))) * HueDegrees1 / 2 ;
    localHSV1[1] = startHSV1[1] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newSat1 - startHSV1[1]) / 2 ;
    localHSV1[2] = (startHSV1[2] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newVal1 - startHSV1[2]) / 2 ) * ValKnob  ;
    localHSV2[0] = startHSV2[0] + (1 - cos(radians(Stepr * 180.0 / Steps))) * HueDegrees2 / 2 ;
    localHSV2[1] = startHSV2[1] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newSat2 - startHSV2[1]) / 2 ;
    localHSV2[2] = (startHSV2[2] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newVal2 - startHSV2[2]) / 2 ) * ValKnob  ;

    HSVcheckLimits(localHSV1);
    HSVcheckLimits(localHSV2);
    HSVwriteToLEDs(localHSV1, fadeZone1);  // << Includes whiteBalance.
    HSVwriteToLEDs(localHSV2, fadeZone2);
    delay( delayKnob(4.4) );         // This delay determines the speed of the fade.  (Why 14???)
  }                           /////////////////////// <<<<<< END FADE process  END FADE.
}

/////////////////////////////////////////////
void HSV_sinFade_Replace_2Z (float Steps,
                             int fadeZone1, float startHSV1[], float HueDegrees1, int newSat1, int newVal1,
                             int fadeZone2, float startHSV2[], float HueDegrees2, int newSat2, int newVal2 ) {
  HSV_sinFade_Retain_2Z (Steps, fadeZone1, startHSV1, HueDegrees1, newSat1, newVal1,
                         fadeZone2, startHSV2, HueDegrees2, newSat2, newVal2);
  //                    Set final values for the fade to pass forward for next fade.
  startHSV1[0] = startHSV1[0] + HueDegrees1;
  startHSV1[1] = newSat1;
  startHSV1[2] = newVal1;
  startHSV2[0] = startHSV2[0] + HueDegrees2;
  startHSV2[1] = newSat2;
  startHSV2[2] = newVal2;
  HSVcheckLimits(startHSV1);
  HSVcheckLimits(startHSV2);
}

/////////////////////////////////////////
//////////////////////////////////////////
void HSVtoHSV_sinFade_Retain_1Z(float Steps, int fadeZone, float startHSV[], float newHSV[]) {

  float HueDiff = (newHSV[0] - startHSV[0]);    // Be sure change of Hue is in range -180 to 180
  if (HueDiff > 180) {
    HueDiff = HueDiff - 360;
  }
  if (HueDiff < -180) {
    HueDiff = HueDiff + 360;
  }
  float localHSV[3];
  float ValKnob;   //  << Variable for Brightness knob.
  for (int Stepr = 0; Stepr <= Steps; Stepr++) {              ///////////// <<<<<<<<<<<<  START FADE ...  START FADE  one extra on purpose.
    ValKnob = brightness();

    localHSV[0] = startHSV[0] + (1 - cos(radians(Stepr * 180.0 / Steps))) * HueDiff / 2 ;
    localHSV[1] = startHSV[1] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newHSV[1] - startHSV[1]) / 2 ;
    localHSV[2] = (startHSV[2] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newHSV[2] - startHSV[2]) / 2 ) * ValKnob ;
    HSVcheckLimits(localHSV);
    HSVwriteToLEDs(localHSV, fadeZone);  // << Includes whiteBalance.
    delay(delayKnob(4.4) );             // This delay determines the speed of the fade.
  }                                      ////////////////// <<<<<< END FADE process  END FADE.
}

/////////////////////////////////////////////

void HSVtoHSV_sinFade_Replace_1Z (float Steps, int fadeZone, float startHSV[], float newHSV[]) {
  HSVtoHSV_sinFade_Retain_1Z (Steps, fadeZone, startHSV, newHSV);
  // Set final values for the fade to pass forward for next fade.
  startHSV[0] = newHSV[0];     // newHSV[3] is the final color of the fade.
  startHSV[1] = newHSV[1];
  startHSV[2] = newHSV[2];

  HSVcheckLimits(startHSV);

}
/////////////
/////////////////////////////////////////////
void HSVtoHSV_sinFade_Retain_2Z(float Steps, int fadeZone1, float startHSV1[], float newHSV1[],
                                int fadeZone2, float startHSV2[], float newHSV2[]) {

  float HueDiff1 = (newHSV1[0] - startHSV1[0]);    // Be sure change of Hue is in range -180 to 180
  float HueDiff2 = (newHSV2[0] - startHSV2[0]);
  if (HueDiff1 > 180) {
    HueDiff1 = HueDiff1 - 360;
  }
  if (HueDiff1 < -180) {
    HueDiff1 = HueDiff1 + 360;
  }
  if (HueDiff2 > 180) {
    HueDiff2 = HueDiff2 - 360;
  }
  if (HueDiff2 < -180) {
    HueDiff2 = HueDiff2 + 360;
  }
  float localHSV1[3];
  float localHSV2[3];
  float ValKnob;   //  << Variable for Brightness knob.
  for (int Stepr = 0; Stepr <= Steps; Stepr++) {              ///////////// <<<<<<<<<<<<  START FADE ...  START FADE  one extra on purpose.
    ValKnob = brightness();

    localHSV1[0] = startHSV1[0] + (1 - cos(radians(Stepr * 180.0 / Steps))) * HueDiff1 / 2 ;
    localHSV1[1] = startHSV1[1] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newHSV1[1] - startHSV1[1]) / 2 ;
    localHSV1[2] = (startHSV1[2] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newHSV1[2] - startHSV1[2]) / 2 ) * ValKnob ;
    localHSV2[0] = startHSV2[0] + (1 - cos(radians(Stepr * 180.0 / Steps))) * HueDiff2 / 2 ;
    localHSV2[1] = startHSV2[1] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newHSV2[1] - startHSV2[1]) / 2 ;
    localHSV2[2] = (startHSV2[2] + (1 - cos(radians(Stepr * 180.0 / Steps))) * (newHSV2[2] - startHSV2[2]) / 2 ) * ValKnob ;
    HSVcheckLimits(localHSV1);
    HSVcheckLimits(localHSV2);
    HSVwriteToLEDs(localHSV1, fadeZone1);   // << Includes whiteBalance.
    HSVwriteToLEDs(localHSV2, fadeZone2);
    delay(delayKnob(4.4) );                // This delay determines the speed of the fade.
  }                                         /////////////////////// <<<<<< END FADE process  END FADE.
}

//////////////////////////////////////////
void HSVtoHSV_sinFade_Replace_2Z (float Steps, int fadeZone1, float startHSV1[], float newHSV1[],
                                  int fadeZone2, float startHSV2[], float newHSV2[]) {
  HSVtoHSV_sinFade_Retain_2Z (Steps, fadeZone1, startHSV1, newHSV1, fadeZone2, startHSV2, newHSV2);
  // Set final values for the fade to pass forward for next fade.
  startHSV1[0] = newHSV1[0];     // newHSV[3] is the final color of the fade.
  startHSV1[1] = newHSV1[1];
  startHSV1[2] = newHSV1[2];
  startHSV2[0] = newHSV2[0];     // newHSV[3] is the final color of the fade.
  startHSV2[1] = newHSV2[1];
  startHSV2[2] = newHSV2[2];
  HSVcheckLimits(startHSV1);
  HSVcheckLimits(startHSV2);
}

//////////////////////////////////////////
//////////////////////////////////////////
void HSVcheckLimits(float hsvArray[])  {
  if (hsvArray[0] < 0) {                    // Check that Hue=startHSVf[0] is within range (0 - 359).
    hsvArray[0] = hsvArray[0] + 360;
  }
  else if (hsvArray[0] >= 360) {          // Hue=360 equals Hue=0.
    hsvArray[0] = hsvArray[0] - 360;
  }
  if (hsvArray[1] < 0) {                  // Keep Saturation within range (0 - 100).
    hsvArray[1] = 0;
  }
  else if (hsvArray[1] > 100) {
    hsvArray[1] = 100;
  }
  if (hsvArray[2] < 0) {                 // Keep Value within range (0 - 100).
    hsvArray[2] = 0;
  }
  else if (hsvArray[2] > 100) {
    hsvArray[2] = 100;
  }
}

//////////////////////////////////////////
void HSVwriteToLEDs(float someHSV[], int someZone) {  // <<< Includes whiteBalance.
  float RGB_from_hsv[3];

  HSVehtoRGB(someHSV, RGB_from_hsv);              // Convert HSV to RGB on every iteration.

  //"White balance" is applied to the RGB form of the color HERE. Use RedScaler if needed.
  // colorRGB[0] = rgbf_color[0] * RedScaler / 100.0;  // RedScaler = 100   //  100.0
  RGB_from_hsv[1] = RGB_from_hsv[1] * GrnScaler / 100.0;  // GrnScaler = 77    //  67.0
  RGB_from_hsv[2] = RGB_from_hsv[2] * BluScaler / 100.0;  // BluScaler = 57    //  37.0


  debug(F("B ")); debug(brightness()); debug(F(" "));
  debug(F("@ ")); debug(someZone);
  debug(F(" V ")); debug( someHSV[2] );
  debug(F(" G ")); debug( RGB_from_hsv[1] ); // amplitude should track V
  debugln();


  for (byte led = 0; led < 3; led++) {
    PWM.set(LunitZones[someZone][led], RGB_from_hsv[led]);  // Display color step.
  }
  PWM.commit(); // for spi/i2c devices, try to batch more updates at once
}
//////////////////////////////////////////
void whiteBalance (float colorRGB[]) {
  //  >>> "White balance"  is applied to the RGB form of the color. Use RedScaler if needed.
  //  colorRGB[0] = rgbf_color[0] * RedScaler / 100.0;  // RedScaler = 100   //  100.0
  colorRGB[1] = colorRGB[1] * GrnScaler / 100.0;  // GrnScaler = 77    //  67.0
  colorRGB[2] = colorRGB[2] * BluScaler / 100.0;  // BluScaler = 57    //  37.0
}

//////////////////////////////////////////////////////////////////

void HSVehtoRGB(float HSVfColor[], float rgbf_color[]) {
  //  Reads values from HSVcolor[]; uses the values to build rgbf_color[].    Slick trick from Alan. <<<<<<
  //  Uses:  HSVehRange(hsvColorF).

  float Hue = HSVfColor[0];  // 0 to 360.       //***** lowercase ....
  float Sat = HSVfColor[1];  // 0 to 100.
  float Val = HSVfColor[2];  // 0 to 100.

  // rgb's are in 0.0 ... 1.0
  int rangeNumber = HSVehRange(HSVfColor);      ////////  Uses: HSVehRange(hsvColorF).  <<<< FIXME: Can I do away with "rangeNumber" ?
  switch (rangeNumber) {
    case 1:    // Range 1 - HuNum = 0 - 31 ... RGB - {255, xx, 0}   (Maj, Mid, Min)
      rgbf_color[0] = Val;                             // Maj
      rgbf_color[1] = Val * (255 - Sat * (255 - 2.845 * Hue) / 100) / 100 / 255; // Mid (punt on 255 thing)
      rgbf_color[2] = Val * (100 - Sat) / 100;         // Min
      break;
    case 2:    // Range 2  HSV[0]=32 to 87. ... RGB - {255, xx, 0}   (Maj, Mid, Min)
      rgbf_color[0] = Val;                                   // Maj
      rgbf_color[1] = Val * (255 - Sat * (255 - 2.268 * (Hue + 11.438)) / 100) / 255; // Mid
      rgbf_color[2] = Val * (100 - Sat) / 100;               // Min
      break;
    case 3:    //Range 3   HSV[0]=88 to 143...  RGB - {XX, 255, 0}     (Mid, Maj, Min)
      rgbf_color[0] = Val * (255 - Sat * (3.0238 * (Hue - 16.669) - 255) / 100) / 255; // Mid
      rgbf_color[1] = Val;                                    // Maj
      rgbf_color[2] = Val * (100 - Sat) / 100;         // Min
      break;
    case 4:   // Range 4    HSV[0]=144 to 162...  RGB - {XX, 255, 0}    (Mid, Maj, Min)
      rgbf_color[0] = Val * (255 - Sat * (6.737 * (Hue - 86.3) - 255) / 100) / 255; // Mid
      rgbf_color[1] = Val;                                     // Maj
      rgbf_color[2] = Val * (100 - Sat) / 100;          // Min
      break;
    case 5:     // Range 5    HSV[0]=163 to 200    RGB - {0, 255, xx}    (Min, Maj, Mid)
      rgbf_color[0] = Val * ( 255 * (100 - Sat) / 100) / 100;       // Min
      rgbf_color[1] = Val;                                   // Maj
      rgbf_color[2] = Val * (255 - Sat * (1342.18 - 6.711 * Hue) / 100) / 255; // Mid
      break;
    case 6:       // Range 6    HSV[0]=201-230    RGB - {0, xx, 255}     (Min, Mid, Maj)
      rgbf_color[0] = Val * (100 - Sat) / 100;       // Min
      rgbf_color[1] = Val * (255 - Sat * (4.233 * Hue - 846.6) / 100) / 255; // Mid
      rgbf_color[2] = Val;                                  // Maj
      break;
    case 7:       // Range 7    HSV[0]=231-254    RGB - {0, xx, 255}     (Min, Mid, Maj)
      rgbf_color[0] = Val * (100 - Sat) / 100;       // Min
      rgbf_color[1] = Val * (255 - Sat * (5.333 * Hue - 1099.59) / 100) / 255; // Mid
      rgbf_color[2] = Val;                                  // Maj
      break;
    case 8:        //Range 8   HSV[0]=255 to 286...  RGB - {xx, 0, 255}     (Mid, Min, Maj)
      rgbf_color[0] = Val * (255 - Sat * (1271 - 4 * Hue) / 100) / 255; // Mid
      rgbf_color[1] = Val * (100 - Sat) / 100;        // Min
      rgbf_color[2] = Val;                                   // Maj
      break;
    case 9:       // Range 9    HSV[0]=287 to 328...  RGB - {xx, 0, 255}     (Mid, Min, Maj)
      rgbf_color[0] = Val * (255 - Sat * (991.86 - 3.024 * Hue) / 100) / 255; // Mid
      rgbf_color[1] = Val * (100 - Sat) / 100;         // Min
      rgbf_color[2] = Val;                                    // Maj
      break;
    case 10:     // Range 10    HSV[0]=329 to 347    RGB - {255, 0, xx}    (Maj, Min, Mid)
      rgbf_color[0] = Val;                                   // Min
      rgbf_color[1] = Val * (100 - Sat) / 100;        // Maj
      rgbf_color[2] = Val * (255 - Sat * (6.684 * Hue - 2192.35) / 100) / 255; // Mid
      break;
    case 11:       // Range 11    HSV[0]=348 TO 360   RGB - {255, 0, xx}    (Maj, Min, Mid)
      rgbf_color[0] = Val;                                   // Min
      rgbf_color[1] = Val * (100 - Sat) / 100;        // Maj
      rgbf_color[2] = Val * (255 - Sat * (9.846 * Hue - 3289.56) / 100) / 255; // Mid
      break;
  }
}

//////////////////////////////////////////
int HSVehRange(float hsvColorF[]) {      // Returns range # 1 - 11, the # of segments in the EH color wheel.
  int rangeNum;
  if (hsvColorF[0] <= 45.0) {        // Range1 -  HuNum = 0 - 31        // Fix me.  Use an array instead of "else if..."
    rangeNum = 1;
  }
  else if (hsvColorF[0] <= 101.0) {       // Range2 -  HuNum = 31 - 86
    rangeNum = 2;
  }
  else if (hsvColorF[0] <= 143.0) {      // Range3 - HuNum = 87 - 142
    rangeNum = 3;
  }
  else if (hsvColorF[0] <= 162.0) {      // Range4 -  HuNum = 143 - 161
    rangeNum = 4;
  }
  else if (hsvColorF[0] <= 200.0) {      // Range5 -  HuNum = 162- 199
    rangeNum = 5;
  }
  else if (hsvColorF[0] <= 230.0) {      // Range6 - HuNum = 200 - 229
    rangeNum = 6;
  }
  else if ( hsvColorF[0] <= 254.0) {      // Range2 -  HuNum = 230 - 253
    rangeNum = 7;
  }
  else if ( hsvColorF[0] <= 286.0) {      // Range3 - HuNum = 254 - 285
    rangeNum = 8;
  }
  else if (hsvColorF[0] <= 328.0) {      // Range4 -  HuNum = 286 - 327
    rangeNum = 9;
  }
  else if (hsvColorF[0] <= 347.0) {      // Range5 -  HuNum = 328 - 346
    rangeNum = 10;
  }
  else if (hsvColorF[0] <= 360.0) {      // Range6 - HuNum = 347 - 359
    rangeNum = 11;
  }
  return rangeNum;
}

/////////////////////////////////////////
//////////////////////////////////////////
void ArrayPrintByte(byte Array[]) {  // Prints any array of three elements.
  Serial.print("   ");
  for (int led = 0; led < 3; led++) {
    Serial.print(Array[led]);
    Serial.print("  ");
  }
}
//////////////////////////////////////////
void ArrayPrintInt(int Array[]) {  // Prints any array of three elements.
  Serial.print("   ");
  for (int led = 0; led < 3; led++) {
    Serial.print(Array[led]);
    Serial.print("  ");
  }
}
//////////////////////////////////////////
void ArrayPrintFloat(float Array[]) {  // Prints any array of three elements.
  Serial.print("   ");
  for (int led = 0; led < 3; led++) {
    Serial.print(Array[led]);
    Serial.print("  ");
  }
}
//////////////////////////////////////////
int delayKnob(float delayNum ) {                  //**** consider the map()     // map( analogRead(),   0,1023, 10,940)
  static unsigned long last = 0;
  static int delay_f;

  if ( millis() - last > 100 ) {
    float knobvalue = (analogRead(SLOWNESS) / 110.0) + 0.1;        // analogRead(3) = 0 to 1023.
    if (BRIGHTNESS == -1) knobvalue = SLOWNESS_OVERRIDE ; // override, force mid speed
    delay_f = delayNum * knobvalue;   // delayKnob(100) = 10 to 940.
    last = millis();
  }

  return delay_f;

}

//////////////////////////////////////
int delayKnobNEW(float delayNum ) {
  float knobvalue = (analogRead(SLOWNESS) / 330.0) + 0.1;       // analogRead(3) = 0 to 1023.
  int delayout = delayNum * knobvalue;                           // delayKnob(100) = 10 to 320.
  return delayout;                                   // 'return' is not a function... It is an statement - no paren needed.

}

//////////////////////////////////////
float brightness() {
  // Needs the BRIGHTNESS pin, a pot
  // Calibrated.
  // Isn't this the same as:
  // map((float) analogRead(BRIGHTNESS), 0.2,1043.9, 0.0,1024.0 )

  // analogRead is slow, so do every 1/10 sec
  static unsigned long last = 0;
  static float brightness_f;

  if ( millis() - last > 100 ) {
    brightness_f = analogRead(BRIGHTNESS) / 1043.9 + 0.02;
    if (BRIGHTNESS == -1) brightness_f = BRIGHTNESS_OVERRIDE ; // override, force mid brightness
    last = millis();
  }

  return brightness_f;
}

//////////////////////////////////////
void randHSVassign  (byte SectorArray[], float finalHSV[]) {
  // Given a sector-array, 'randHSVassign' produces a random HSV color point in that sector.
  // SectorArray[] designates the sectors from which finalHSV will be constructed.
  // finalHSV[] holds the final destination HSV.
  finalHSV[0] = random (HUeRANGES[SectorArray[0] - 1] + 1, HUeRANGES [SectorArray[0]] + 1);
  finalHSV[1] = random (SAtRANGES[SectorArray[1] - 1] + 1, SAtRANGES [SectorArray[1]] + 1);
  finalHSV[2] = random (VAlRANGES[SectorArray[2] - 1] + 1, VAlRANGES [SectorArray[2]] + 1);
}
/////////////////////////////
void randomSectorArray(byte someSector[]) { // Creates a random array of sectors.
  for (byte js = 0; js < 3; js++) {
    someSector[js] = random (1, 7);
  }
}
///////////////////////////
