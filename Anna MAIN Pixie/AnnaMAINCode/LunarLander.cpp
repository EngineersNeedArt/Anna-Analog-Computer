// =============================================================================
//                                LunarLander.cpp
// =============================================================================

#include <Arduino.h>
#include "LunarLander.h"
#include "Utilities.h"


#define LANDER_X                172
#define LANDER_SPRITE_WIDTH     34
#define LANDER_SPRITE_HEIGHT    25
#define LANDER_ORIGIN_X         17
#define LANDER_ORIGIN_Y         10
#define LL_LABEL_SPRITE_WIDTH   120
#define LL_LABEL_SPRITE_HEIGHT  14

#define LIGHT_CRT               TFT_GREEN // 0x7FEF
#define DARK_CRT                0x05C0    // TFT_GREEN

#define TOUCHDOWN_ALTITUDE      118
#define SAFE_VELOCITY           -0.10

#define LANDER_ALPHA            0.50f


TFT_eSprite *landerSprite = nullptr;
TFT_eSprite *gravitySprite = nullptr;
TFT_eSprite *thrustSprite = nullptr;
TFT_eSprite *velocitySprite = nullptr;
TFT_eSprite *fuelSprite = nullptr;
bool landed = false;
bool safeLanding = false;
float crashedVelocity = 0.0;
float previousVelocityHack = 0.0;


// =================================================================== Functions
// ------------------------------------------------------------------- runLunarLander

// Sources: 1: gravity, 2: thrust, 3: velocity, 4: elevation, 5: fuel level

void runLunarLander (struct MeterData *data) {
  // Black sky.
  data->buffer->fillSprite (TFT_BLACK);
  
  // Moon surface.
  data->buffer->drawRect (0, data->displayHeight - 2, data->displayWidth, 2, LIGHT_CRT);

  char textBuffer[8];

  // Display gravity amount.
  sprintf (textBuffer, "%.0f", clippedMachineValue (data->source1, NULL) * 100.0f);
  gravitySprite->setTextDatum (TR_DATUM);
  gravitySprite->setTextColor (LIGHT_CRT, TFT_BLACK);
  gravitySprite->setTextPadding (50);
  gravitySprite->drawString (textBuffer, LL_LABEL_SPRITE_WIDTH, 0);
  gravitySprite->pushToSprite (data->buffer, 0, 0);

  // Display thrust amount.
  float thrust = clippedMachineValue (data->source2, NULL) * 100.0f;
  if (thrust < 0.0) {
    thrust = 0.0;
  }
  dtostrf (thrust, 0, 0, textBuffer);
  thrustSprite->setTextDatum (TR_DATUM);
  thrustSprite->setTextColor (LIGHT_CRT, TFT_BLACK);
  thrustSprite->setTextPadding (50);
  thrustSprite->drawString (textBuffer, LL_LABEL_SPRITE_WIDTH, 0);
  thrustSprite->pushToSprite (data->buffer, 0, LL_LABEL_SPRITE_HEIGHT + 4);

  // Display velocity amount.
  float displayedVelocity = -(clippedMachineValue (data->source3, NULL));

  if (landed) {
    if (safeLanding) {
      displayedVelocity = 0;
    } else {
      displayedVelocity = crashedVelocity;
    }
  }
  if ((displayedVelocity >= 0.0f) && (!landed)) {
    textBuffer[0] = '+';
    dtostrf (displayedVelocity * 100.0f, 0, 0, textBuffer + 1);
  } else {
    dtostrf (displayedVelocity * 100.0f, 0, 0, textBuffer);
  }
  velocitySprite->setTextDatum (TR_DATUM);
  velocitySprite->setTextColor (LIGHT_CRT, TFT_BLACK);
  velocitySprite->setTextPadding (50);
  velocitySprite->drawString (textBuffer, LL_LABEL_SPRITE_WIDTH, 0);
  velocitySprite->pushToSprite (data->buffer, 0, (LL_LABEL_SPRITE_HEIGHT + 4) * 2);

  // Display fuel amount.
  int fuelPercentage = roundf ((clippedMachineValue (data->source5, NULL) + 1.0) * 50.0f);
  sprintf (textBuffer, "%d%%", fuelPercentage);
  fuelSprite->setTextDatum (TR_DATUM);
  fuelSprite->setTextColor (LIGHT_CRT, TFT_BLACK);
  fuelSprite->setTextPadding (50);
  fuelSprite->drawString (textBuffer, LL_LABEL_SPRITE_WIDTH, 0);
  fuelSprite->pushToSprite (data->buffer, 0, (LL_LABEL_SPRITE_HEIGHT + 4) * 3);

  // Hack to determine if Thrust is on. (Will fix with a future PCB.)
  bool thrustIsOnHack = false;  
  if (previousVelocityHack != 0.0) {
    thrustIsOnHack = (displayedVelocity > previousVelocityHack);
  }
  previousVelocityHack = displayedVelocity;

  int landerY = roundf ((data->displayHeight / 2.0) * (1.0 - clippedMachineValue (data->source4, NULL)));
  if (landerY > TOUCHDOWN_ALTITUDE) {
    if (!landed) {
      // Serial.println (displayedVelocity);
      landed = true;
      safeLanding = (displayedVelocity > SAFE_VELOCITY);
      if (!safeLanding) {
        crashedVelocity = displayedVelocity;
      }
    }
    landerY = TOUCHDOWN_ALTITUDE;
  } else if ((landed) && (landerY < TOUCHDOWN_ALTITUDE)) {
    landed = false;
  }
  
  landerSprite->pushToSprite (data->buffer, LANDER_X - LANDER_ORIGIN_X, landerY - LANDER_ORIGIN_Y);

  // Draw thrust (if on).
  if ((thrustIsOnHack) && (!landed)) {
    data->buffer->drawLine (LANDER_X - 5, landerY + 12, LANDER_X, landerY + 40, LIGHT_CRT);
    data->buffer->drawLine (LANDER_X, landerY + 40, LANDER_X + 5, landerY + 12, LIGHT_CRT);
  }
  
  if ((landed) || (fuelPercentage <= 0)) {
      data->buffer->setTextFont (1);
      data->buffer->setTextSize (3);
      data->buffer->setTextDatum (CC_DATUM);
      data->buffer->setTextColor (LIGHT_CRT, TFT_BLACK);
    if (safeLanding) {
      data->buffer->drawString ("LANDED!", data->displayWidth / 2.0, (data->displayHeight / 2.0) + 20);
    } else if (landed) {
      data->buffer->drawString ("CRASHED!", data->displayWidth / 2.0, (data->displayHeight / 2.0) + 20);
    } else {
      data->buffer->drawString ("NO FUEL!", data->displayWidth / 2.0, (data->displayHeight / 2.0) + 20);
    }
    data->buffer->setTextSize (2);
    // data->buffer->setTextColor (DARK_CRT, TFT_BLACK);
    data->buffer->drawString ("(Reset INT)", (data->displayWidth / 2.0) - 50, (data->displayHeight / 2.0) + 50);
  }

  data->buffer->pushSprite (0, 0);
}

// ------------------------------------------------------------------- createLanderSprite

TFT_eSprite *createLanderSprite (TFT_eSPI *tft) {  
  TFT_eSprite *sprite = new TFT_eSprite (tft);
  
  // Create sprite.
  sprite->setColorDepth (16);
  sprite->createSprite (LANDER_SPRITE_WIDTH, LANDER_SPRITE_HEIGHT);

  // Background of sprite.
  sprite->fillSprite (TFT_BLACK);
  
  sprite->drawEllipse (LANDER_ORIGIN_X, LANDER_ORIGIN_Y - 4, 8, 6, DARK_CRT);
  sprite->drawRect (LANDER_ORIGIN_X - 8, LANDER_ORIGIN_Y + 2, 16, 8, DARK_CRT);

  sprite->drawLine (LANDER_ORIGIN_X - 8, LANDER_ORIGIN_Y + 4, LANDER_ORIGIN_X - 8 - 6, LANDER_ORIGIN_Y + 4 + 6, DARK_CRT);
  sprite->drawLine (LANDER_ORIGIN_X - 8 - 6, LANDER_ORIGIN_Y + 4 + 6, LANDER_ORIGIN_X - 8 - 6, LANDER_ORIGIN_Y + 4 + 10, DARK_CRT);
  sprite->drawFastHLine (LANDER_ORIGIN_X - 8 - 6 - 2, LANDER_ORIGIN_Y + 4 + 10, 4, DARK_CRT);

  sprite->drawLine (LANDER_ORIGIN_X + 8, LANDER_ORIGIN_Y + 4, LANDER_ORIGIN_X + 8 + 6, LANDER_ORIGIN_Y + 4 + 6, DARK_CRT);
  sprite->drawLine (LANDER_ORIGIN_X + 8 + 6, LANDER_ORIGIN_Y + 4 + 6, LANDER_ORIGIN_X + 8 + 6, LANDER_ORIGIN_Y + 4 + 10, DARK_CRT);
  sprite->drawFastHLine (LANDER_ORIGIN_X + 8 + 6 - 2, LANDER_ORIGIN_Y + 4 + 10, 4, DARK_CRT);

  return sprite;
}

// ------------------------------------------------------------------- createLunarLabelSprite

TFT_eSprite *createLunarLabelSprite (TFT_eSPI *tft, const char *label) {  
  TFT_eSprite *sprite = new TFT_eSprite (tft);

  // Create sprite.
  sprite->setColorDepth (16);
  sprite->createSprite (LL_LABEL_SPRITE_WIDTH, LL_LABEL_SPRITE_HEIGHT);

  // Background of sprite.
  sprite->fillSprite (TFT_BLACK);

  // Draw label portion.
  sprite->setTextFont (1);
  sprite->setTextSize (2);
  sprite->setTextDatum (TL_DATUM);
  sprite->setTextColor (DARK_CRT, TFT_BLACK);
  sprite->drawString (label, 0, 0);

  return sprite;
}

// ------------------------------------------------------------------- prepareLunarLander

float prepareLunarLander (TFT_eSPI *tft) {
  landerSprite = createLanderSprite (tft);
  gravitySprite = createLunarLabelSprite (tft, "GRAV:");
  thrustSprite = createLunarLabelSprite (tft, "THRST:");
  velocitySprite = createLunarLabelSprite (tft, "VELO:");
  fuelSprite = createLunarLabelSprite (tft, "FUEL:");

  landed = false;
  safeLanding = false;
  crashedVelocity = 0.0;
  previousVelocityHack = 0.0;

  return LANDER_ALPHA;
}

// ------------------------------------------------------------------- cleanupLunarLander

void cleanupLunarLander () {
  if (landerSprite) {
    landerSprite->deleteSprite ();
    delete landerSprite;
    landerSprite = nullptr;
  }
  if (gravitySprite) {
    gravitySprite->deleteSprite ();
    delete gravitySprite;
    gravitySprite = nullptr;
  }
  if (thrustSprite) {
    thrustSprite->deleteSprite ();
    delete thrustSprite;
    thrustSprite = nullptr;
  }
  if (velocitySprite) {
    velocitySprite->deleteSprite ();
    delete velocitySprite;
    velocitySprite = nullptr;
  }
  if (fuelSprite) {
    fuelSprite->deleteSprite ();
    delete fuelSprite;
    fuelSprite = nullptr;
  }
}
