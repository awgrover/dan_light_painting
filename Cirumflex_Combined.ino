//Fixed colors set as globals. 03-Pattern fades between them.
const int RANDOMnoisePIN = A5; // for random seed
const int BRIGHTNESS = 1;      // Value pot pin number
const int SLOWNESS = 3;        // Pot pin number; 0 is fast, 1023 is slow.

// Which LED interface?
#include "PWM_TLC59711.h"
PWM_TLC59711 PWM;

// Pins for LEDs in each zone
// You have to go look in the PWM class to figure out the physical pin
// On the mega, pwm1 is pin 2, and increments till pwm12
const byte aLunitPins[3] = {PWM.pwm12, PWM.pwm11, PWM.pwm10};    // Zone A    >> Zone 1   {R, G, B}
const byte bLunitPins[3] = {PWM.pwm1, PWM.pwm2, PWM.pwm3};       // Zone B    >> Zone 2   {R, G, B}
const byte cLunitPins[3] = {PWM.pwm7, PWM.pwm8, PWM.pwm9};      // Zone C    >> Zone 3   {R, G, B}
const byte dLunitPins[3] = {PWM.pwm6, PWM.pwm5, PWM.pwm4};       // Zone D    >> Zone 4   {R, G, B}
const byte *LunitZones[4] = {aLunitPins, bLunitPins, cLunitPins, dLunitPins}; // <<< fadeZone = 1, 2. 3, 4
// Percent scaling for R vs G vs B for "White Balance" empirical values for LED strips. Used in HSVehtoRGB(HSVfColor, rgbf_color)
// So, if Red is 100% there is no need for >>>  const float RednScaler = 100.0;
const float GrnScaler = 42.0;   // <<Wht balance Entrance.  << 9/2018 - 80% 75%
const float BluScaler = 72.0;   // <<Wht balance Entrance.  << 9/2018 - 65% 55%
//  /*  ////
const float Ahsv[3] = { 3, 100, 100};
const float Bhsv[3] = { 49, 100, 100};
const float Chsv[3] = {92, 100, 100};
const float Dhsv[3] = {146, 100, 100} ;
const float Ehsv[3] = {193, 100, 100};
const float Fhsv[3] = {242, 100, 100} ;
const float Ghsv[3] = {291, 100, 100} ;
const float Hhsv[3] = {324, 100, 100} ;
float AinitHSV[3] ; // = {180, 100, 70};    // {20, 80, 100};  // Start HSV-points for each zone.
float BinitHSV[3] ; // = {90, 100, 100};
float CinitHSV[3] ; // = {270, 100, 70};    // {180, 100, 70};
float DinitHSV[3] ; // = {20, 80, 100};     // {270, 100, 70};
float *initHSV[4] = {AinitHSV, BinitHSV, CinitHSV, DinitHSV};
float AfiniHSV[3] = {60, 0, 50} ;                  // End HSV-points for each zone.
float BfiniHSV[3] = {60, 0, 50} ;
float CfiniHSV[3] = {60, 0, 50} ;
float DfiniHSV[3] = {60, 0, 50} ;
float *finiHSV[4] = {AfiniHSV, BfiniHSV, CfiniHSV, DfiniHSV};
int *zoneNum[4] = {0, 1, 2, 3};
byte Asectors[3] = {1, 6, 5} ;            // Sector-arrays for each zone.
byte Bsectors[3] = {2, 6, 5} ;   // Change sector names to (0, 1, 2, 3, 4, 5)
byte Csectors[3] = {4, 6, 5} ;
byte Dsectors[3] = {5, 6, 5} ;
byte *Ssectors[4] = {Asectors, Bsectors, Csectors, Dsectors};
const int HUeRANGES[] = {0, 60, 120, 180, 240, 300, 360};
const int SAtRANGES[] = {0, 33,  58,  75,  86,  94, 100};
const int VAlRANGES[] = {0, 33,  58,  75,  86,  94, 100};
//int i = 0;   // zone   variable for zones.
//int j = 0;   // leds   variable for RGB LEDs in each zone.
//int k = 0;   // pwmv   variable for pwm value for each LED-color.
//int n = 1;
byte ZoneSet[4] = {1, 2, 3, 4}; // >>>change to: byte ZoneSet[4] = {0, 1, 2, 3};
/////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(RANDOMnoisePIN));
  for (byte led = 0; led < 3; led++) {
    PWM.begin(aLunitPins[led]);
    PWM.begin(bLunitPins[led]);
    PWM.begin(cLunitPins[led]);
    PWM.begin(dLunitPins[led]);
  }
  pinMode(BRIGHTNESS, INPUT);        // pin 1 is the Value (value) knob.
  pinMode(SLOWNESS, INPUT);          // pin 3 is the slowness knob.
  //  This pattern(01) is placed in the "setup" section so it is used once.
  Serial.println(   "Starting Startup");
  // randHSVassign  (Asectors, AfiniHSV) ;
  //  randHSVassign  (Csectors, CfiniHSV) ;
  HSVtoHSV_sinFade_Replace_2Z (500, 0, AinitHSV, AfiniHSV, 2, CinitHSV, CfiniHSV);
  //  randHSVassign  (Bsectors, BfiniHSV) ;
  //  randHSVassign  (Dsectors, DfiniHSV) ;
  HSVtoHSV_sinFade_Replace_2Z (500, 1, BinitHSV, BfiniHSV, 3, DinitHSV, DfiniHSV);

}
////////////////////////////////////////////////////////

void loop() {
  byte m;
  byte i;
  byte cycle = random(0, 4);
  switch (cycle) {
    case 0:
      m = random(2, 4);
      Serial.print(   "Starting pattern03_FixedColorWalk  m = ");
      Serial.println(m);
      for (i = 0; i < m; i++) {
        Serial.print(i);
        pattern03_FixedColorWalk();
      }
      Serial.println();
      break;
    case 1:
      m = random(4, 15);
      Serial.print(   "Starting pattern04_CustomFades  m = ");
      Serial.println(m);
      for (i = 0; i < m; i++) {
        Serial.print(i);
        pattern04_CustomFades ();
      }
      Serial.println();
      break;
    case 2:
      Serial.println(   "Starting pattern05_Randomizer ");
      pattern05_Randomizer();
      break;
    case 3:
      m = random(2, 7);
      Serial.print(   "Starting pattern06_loSatValShifter  m = ");
      Serial.println(m);
      for (i = 0; i < m; i++) {
        Serial.print(i);
        pattern06_loSatValShifter();
      }
      Serial.println();
      break;

  }
  delayKnob(2000);

  // pattern03_FixedColorWalk();
  // pattern04_CustomFades ();
  // pattern05_Randomizer();
  // pattern06_loSatValShifter( );
}

//////////////////////////// rev. June 18, 2019 ///  How do I get in and out of each pattern??????
void pattern03_FixedColorWalk ( ) {         // Change name to pattern03_SatColorWalk
  int rando = random(-120, 120);
  float aahsv[3] = {Ahsv[0] + rando, 100, 100};   // Test for Hue range? Yes.
  HSVcheckLimits(aahsv);
  float bbhsv[3] = {Bhsv[0] + rando, 100, 100};
  HSVcheckLimits(bbhsv);
  float cchsv[3] = {Chsv[0] + rando, 100, 100};
  HSVcheckLimits(cchsv);
  float ddhsv[3] = {Dhsv[0] + rando, 100, 100};
  HSVcheckLimits(ddhsv);
  float eehsv[3] = {Ehsv[0] + rando, 100, 100};
  HSVcheckLimits(eehsv);
  float ffhsv[3] = {Fhsv[0] + rando, 100, 100};
  HSVcheckLimits(ffhsv);
  float gghsv[3] = {Ghsv[0] + rando, 100, 100};
  HSVcheckLimits(gghsv);
  float hhhsv[3] = {Hhsv[0] + rando, 100, 100};
  HSVcheckLimits(hhhsv);

  HSVtoHSV_sinFade_Replace_2Z (500, 0, AinitHSV, bbhsv, 2, CinitHSV, ddhsv);
  HSVtoHSV_sinFade_Replace_2Z (500, 1, BinitHSV, cchsv, 3, DinitHSV, ffhsv);
  delayKnob(900);
  // Serial.println("  step 1: Zone0 : B to H.");                  // Ahsv = (3, 100, 100)
  HSVtoHSV_sinFade_Replace_1Z(500, 0, AinitHSV, hhhsv);   // Bhsv = (49, 100, 100)
  // Serial.println("   Zone1 : C to E.");                  // Chsv = (92, 100, 100)
  HSVtoHSV_sinFade_Replace_1Z(500, 1, BinitHSV, eehsv);  // Dhsv = (146, 100, 100)
  // Serial.println("   Zone2 : D to B.");                  // Ehsv = (193, 100, 100)
  HSVtoHSV_sinFade_Replace_1Z(500, 2, CinitHSV, bbhsv);   // Fhsv = (242, 100, 100)
  // Serial.println("   Zone3 : F to A. Pause.");           // Ghsv = (291, 100, 100)
  HSVtoHSV_sinFade_Replace_1Z(500, 3, DinitHSV, aahsv);  // Hhsv = (324, 100, 100)
  // Serial.println();
  delayKnob(500);

  // Serial.println("  step 2: Zone0 : H to D.");
  HSVtoHSV_sinFade_Replace_1Z(500, 0, AinitHSV, ddhsv);
  // Serial.println("   Zone1 : E to G.");
  HSVtoHSV_sinFade_Replace_1Z(500, 1, BinitHSV, gghsv);
  // Serial.println("   Zone2 : B to F.");
  HSVtoHSV_sinFade_Replace_1Z(500, 2, CinitHSV, ffhsv);
  // Serial.println("   Zone3 : A to E.  Pause");
  HSVtoHSV_sinFade_Replace_1Z(500, 3, DinitHSV, eehsv);
  // Serial.println();
  delayKnob(500);

  // Serial.println("  step 3: Zone0 : D to C.");
  HSVtoHSV_sinFade_Replace_1Z(500, 0, AinitHSV, cchsv);
  // Serial.println("   Zone1 : G to B.");
  HSVtoHSV_sinFade_Replace_1Z(500, 1, BinitHSV, aahsv);
  // Serial.println("   Zone2 : F to H.");
  HSVtoHSV_sinFade_Replace_1Z(500, 2, CinitHSV, hhhsv);
  // Serial.println("   Zone3 : E to G.  Pause");
  HSVtoHSV_sinFade_Replace_1Z(500, 3, DinitHSV, gghsv);
  // Serial.println();
  delayKnob(500);

  // Serial.println("  step 4: Zone0 : C to B.");
  HSVtoHSV_sinFade_Replace_1Z(200, 0, AinitHSV, bbhsv);
  // Serial.println("   Zone1 : B to C.");
  HSVtoHSV_sinFade_Replace_1Z(400, 1, BinitHSV, cchsv);
  // Serial.println("   Zone2 : H to D.");
  HSVtoHSV_sinFade_Replace_1Z(600, 2, CinitHSV, ddhsv);
  // Serial.println("   Zone3 : G to F.  Pause");
  HSVtoHSV_sinFade_Replace_1Z(500, 3, DinitHSV, ffhsv);
  // Serial.println();
  delayKnob(500);
}



////////////////////////////
void pattern04_CustomFades ( ) {        // rev 2019-06-17
  int posHueShift = random(47, 114);    //  93;
  int negHueShift = random(-153, -76);  // -139;
  // Serial.print("  posHueShift = ");  Serial.print(posHueShift);
  // Serial.print("  negHueShift = ");  Serial.println(negHueShift);
  // Serial.print(" Part I z0 change ");
  HSV_sinFade_Replace_1Z (500, 0, AinitHSV, posHueShift, 100, 70);  // Z-0
  delayKnob(300); // Serial.print(" z1 change ");
  HSV_sinFade_Replace_1Z (400, 1, BinitHSV, posHueShift, 45, 100);  //   Z-1
  delayKnob(300); // Serial.print(" z3 change ");
  HSV_sinFade_Replace_1Z (600, 3, DinitHSV, posHueShift, 100, 20);  //       Z-3
  delayKnob(300); // Serial.println(" z2 change ");
  HSV_sinFade_Replace_1Z (500, 2, CinitHSV, negHueShift, 100, 88);  //     Z-2
  delayKnob(300);

  // Serial.print("  Part II z1 change ");
  HSV_sinFade_Replace_1Z (500, 1, BinitHSV, posHueShift, 100, 16);  //   Z-1
  delayKnob(300);  // Serial.print("  z0 change ");
  HSV_sinFade_Replace_1Z (300, 0, AinitHSV, negHueShift, 65, 100);  // Z-0
  delayKnob(300);  // Serial.print("  z3 change ");
  HSV_sinFade_Replace_1Z (700, 3, DinitHSV, posHueShift, 98, 65);   //       Z-3
  delayKnob(300);  // Serial.println("  z2 change ");
  HSV_sinFade_Replace_1Z (400, 2, CinitHSV, posHueShift, 95, 85);   //     Z-2
  delayKnob(300);

  // Serial.print("   Part III z3 change ");
  HSV_sinFade_Replace_1Z (200, 3, DinitHSV, posHueShift, 89, 100);  //       Z-3
  delayKnob(300);  // Serial.print("   z0 change ");
  HSV_sinFade_Replace_1Z (600, 0, AinitHSV, negHueShift, 99, 33);   // Z-0
  delayKnob(300);  // Serial.print("   z2 change ");
  HSV_sinFade_Replace_1Z (400, 2, CinitHSV, negHueShift, 75, 99);   //     Z-2
  // Serial.println("   z1 change ");
  HSV_sinFade_Replace_1Z (500, 1, BinitHSV, posHueShift, 100, 62);   //   Z-1
  delayKnob(300);

  // Serial.print("    Part IV  z2 change ");
  HSV_sinFade_Replace_1Z (400, 2, CinitHSV, negHueShift, 100, 13);   //     Z-2
  //  Serial.print("    z3 change ");
  HSV_sinFade_Replace_1Z (600, 3, DinitHSV, posHueShift, 86, 96);   //       Z-3
  //  Serial.print("    z1 change ");
  HSV_sinFade_Replace_1Z (500, 1, BinitHSV, negHueShift, 100, 38);  //   Z-1
  // Serial.println("    z0 change ");
  HSV_sinFade_Replace_1Z (300, 0, AinitHSV, posHueShift, 55, 100);  // Z-0
  delayKnob(300);
  //  Serial.println();

}
/////////////
// This function creates a random set of  ABCD-finiHSV[] color arrays.
// To be used afer a preset set of changes to start another series in different sectors.
void pattern05_Randomizer (  ) {
  AfiniHSV[0] = random(110, 199);
  AfiniHSV[1] = random(65, 95);
  AfiniHSV[2] = random(85, 98);
  BfiniHSV[0] = random(2, 103);
  BfiniHSV[1] = random(25, 90);
  BfiniHSV[2] = random(5, 45);
  CfiniHSV[0] = random(90, 220);
  CfiniHSV[1] = random(35, 65);
  CfiniHSV[2] = random(65, 88);
  DfiniHSV[0] = random(280, 360);
  DfiniHSV[1] = random(85, 100);
  DfiniHSV[2] = random(15, 85);
  //  Serial.print("   AfiniHASV = "); ArrayPrintFloat(AfiniHSV);  Serial.println();
  //  Serial.print("   BfiniHASV = "); ArrayPrintFloat(BfiniHSV);  Serial.println();
  //  Serial.print("   CfiniHASV = "); ArrayPrintFloat(CfiniHSV);  Serial.println();
  //  Serial.print("   DfiniHASV = "); ArrayPrintFloat(DfiniHSV);  Serial.println(); Serial.println();
  HSVtoHSV_sinFade_Replace_2Z (350, 0, AinitHSV, AfiniHSV, 2, CinitHSV, CfiniHSV);
  HSVtoHSV_sinFade_Replace_2Z (350, 1, BinitHSV, BfiniHSV, 3, DinitHSV, DfiniHSV);
  delayKnob(300);
}

////////////////////////////////////

void pattern06_loSatValShifter( ) {
  // Similar to pattern04_CustomFades.. but with low saturation so light and dark are enphasized.
  // Sat ranges    0-25 pale,   25-50 pastel,   50-80 med,     80-100 sat.
  // Value ranges  0-20 dark,   20-45 mid,      45-80 light,   80-100 bright.
  //  Serial.print("   Part 1: 1B & 3D change: ");
  int hueChange1 = random(40, 90);
  int hueChange2 = random(70, 140);
  HSV_sinFade_Replace_2Z (350, 1, BinitHSV,  hueChange2, 24, 100, 3, DinitHSV, -hueChange2, 40, 6);     //   0A   sat   dark
  //  Serial.println("     0A & 2C change: ");                                                            //    1B  pale  bright
  HSV_sinFade_Replace_2Z (400, 0, AinitHSV, -hueChange1, 97,  6, 2, CinitHSV,  hueChange2, 90, 19);    //   2C   sat   dark
  delayKnob(300);                                                                                         //    3D  pastl  dark
  //  Serial.println();

  // Serial.print("   Part 2: 1B & 2C change: ");
  hueChange1 = random(40, 90);
  hueChange2 = random(70, 140);
  HSV_sinFade_Replace_2Z (310, 1, BinitHSV, -hueChange1, 15, 60, 2, CinitHSV, -hueChange1, 30, 8);     //  0A  sat   dark
  //  Serial.println("     0A & 3D change: ");                                                            //   1B  pale  light
  HSV_sinFade_Replace_2Z (380, 0, AinitHSV,  hueChange1, 97, 10, 3, DinitHSV,  hueChange2, 70, 90);   //   2C  pastl  dark
  delayKnob(300);                                                                                           //  3D  med   bright
  //  Serial.println();

  //  Serial.print("   Part 3: 0A & 1B change: ");
  hueChange1 = random(40, 90);
  hueChange2 = random(70, 140);
  HSV_sinFade_Replace_2Z (400, 0, AinitHSV,  hueChange2, 12, 98, 1, BinitHSV, -hueChange1, 20, 4);   //   0A  pale  bright
  //  Serial.println("     2C & 3D change: ");                                                            //   1B  pale   dark
  HSV_sinFade_Replace_2Z (310, 2, CinitHSV,  hueChange2, 97, 73, 3, DinitHSV,  hueChange1, 9, 28);      //  2C  med    dark
  delayKnob(300);                                                                                           //  3D  pale   med
  //  Serial.println();

  //  Serial.print("   Part 4: 0A & 3D change: ");
  hueChange1 = random(40, 90);
  hueChange2 = random(70, 140);
  HSV_sinFade_Replace_2Z (330, 0, AinitHSV, -hueChange1, 17, 16, 3, DinitHSV,  hueChange2, 60, 60);   //   0A  pale  dark
  //  Serial.println("     1B & 2C change: ");                                                              //  1B  sat   dark
  HSV_sinFade_Replace_2Z (330, 1, BinitHSV, -hueChange1, 95, 3, 2, CinitHSV, -hueChange1, 7, 95);      //  2C  pale  bright
  delayKnob(300);                                                                                         //   3D  med   light
  // Serial.println();
}
