// A simple mechanism to print info when debugging
// and easily turn it off everywhere

// Uncomment to turn on, Comment to turn off:
#define DEBUG

// FIXME: use the template style so we get formatting args

#ifdef DEBUG
  #define debug(x) Serial.print(x)
  #define debugln() Serial.println();
#else
  #define debug(x)
#endif
