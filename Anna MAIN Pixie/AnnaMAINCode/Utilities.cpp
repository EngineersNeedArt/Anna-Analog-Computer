// =============================================================================
//                                Utilities.cpp
// =============================================================================

#include <Arduino.h>
#include "Utilities.h"


#define VMIN_CLIP -1.001f
#define VMAX_CLIP 1.001f

#define BUTTON1_PIN    35  // Top button on ESP32 board.
#define BUTTON2_PIN    0   // Bottom button on ESP32 board.


bool button1StillPressed = false;
bool button2StillPressed = false;

// =================================================================== Functions
// ------------------------------------------------------------------- topButtonPressed

bool topButtonPressed () {
  if (digitalRead (BUTTON1_PIN) == LOW) {
    if (!button1StillPressed) {
      button1StillPressed = true;
      return true;
    }
  } else {
    button1StillPressed = false;
  }
  return false;
}

// ------------------------------------------------------------------- bottomButtonPressed

bool bottomButtonPressed () {
  if (digitalRead (BUTTON2_PIN) == LOW) {
    if (!button2StillPressed) {
      button2StillPressed = true;
      return true;
    }
  } else {
    button2StillPressed = false;
  }
  return false;
}

// ------------------------------------------------------------------- machineValueOverloaded

bool machineValueOverloaded (float value) {
    return (value > VMAX_CLIP) || (value < VMIN_CLIP);
}

// ------------------------------------------------------------------- clippedMachineValue

float clippedMachineValue (float sourceValue, bool *clipped) {
  float adjustedValue = sourceValue;

  if (adjustedValue < -1.0) {
    adjustedValue = -1.0;
  } else if (adjustedValue > 1.0) {
    adjustedValue = 1.0;
  }
  
  if (clipped) {
    *clipped = machineValueOverloaded (sourceValue);
  }

  return adjustedValue;
}

// ------------------------------------------------------------------- initUtilities

void initUtilities () {
  pinMode (BUTTON1_PIN, INPUT);
  pinMode (BUTTON2_PIN, INPUT_PULLUP);
}