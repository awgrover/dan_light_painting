#pragma once

class PWM_NativePin {
  // interface for PWM's on the native pins
  // Acts like an object, but no storage, and inlines, so, efficient

  public:

    // For each pwm (e.g. pinMode())
    // Sadly, no sanity checks
    boolean begin(int pin) { 
      pinMode(pin, OUTPUT); 
      return true; 
    }

    // Give it a pin and int and you get PWM
    static void set(int pin, int state) { analogWrite(pin, state); }
    static void set(int pin, float state) { set(pin, (int) state); } // tolerate float
  };

class PWM_NativePin_MEGA : public PWM_NativePin {
    public:

      // pin names
      enum {
        pwm1=2, pwm2, pwm3, pwm4, pwm5, pwm6, pwm7, pwm8, pwm9, pwm10, pwm11, pwm12, 
        pwm13=44, pwm14=45, pwm15=46
        };

      //boolean begin(int pin) { PWM_NativePin::begin(pin); }
    };
