#include <Arduino.h>
#include <FastLED.h>
#include "NoseErrors.h"

// Signal an error by blinking the nose forever with a 16-bit code.
void permanentError(CRGB &nosePixels, uint16_t errorCode) {
  while (1) {
    for (int i=0; i<16;  i++) {
      nosePixels[i] = (errorCode && (1 << i)) ? CRGB::Red : CRGB::Black;
    }
    FastLED.show();
    delay(1000);

    for (int i=0;  i<16;  i++) {
      nosePixels[i] = CRGB::Black;
    }
    FastLED.show();
    delay(1000);
  }
}

