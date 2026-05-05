// =============================================================================
//                              VerticalMeters.cpp
// =============================================================================

#include <Arduino.h>
#include "Utilities.h"
#include "VerticalMeters.h"


#define VMIN_CLIP -1.001f
#define VMAX_CLIP 1.001f

#define VMETER_WIDTH 44
#define VMETER_HEIGHT 634 // VMETER_PIXEL_RANGE + (135) // was 635
#define VMETER_PIXEL_RANGE 500
#define VMETER_V_MARGIN 20

#define VMETER_A_X 2
#define VMETER_B_X 50
#define VMETER_C_X 98
#define VMETER_D_X 146
#define VMETER_E_X 194
#define VMETER_LABEL_X_OFFSET 8
#define VMETER_LABEL_Y_OFFSET 107

#define VMETER_ALPHA     0.50f


TFT_eSprite *vMeterSprite = nullptr;

TFT_eSprite *vLabel1Sprite = nullptr;
TFT_eSprite *vLabel2Sprite = nullptr;
TFT_eSprite *vLabel3Sprite = nullptr;
TFT_eSprite *vLabel4Sprite = nullptr;
TFT_eSprite *vLabel5Sprite = nullptr;

TFT_eSprite *vLabel1OverSprite = nullptr;
TFT_eSprite *vLabel2OverSprite = nullptr;
TFT_eSprite *vLabel3OverSprite = nullptr;
TFT_eSprite *vLabel4OverSprite = nullptr;
TFT_eSprite *vLabel5OverSprite = nullptr;

float vMeterDisplayableRange = ((float) VMETER_PIXEL_RANGE / 2.0) + (float) VMETER_V_MARGIN;

// =================================================================== Functions
// ------------------------------------------------------------------- meterYOffsetForMachineValue

int meterYOffsetForMachineValue (float value, int displayHeight) {
  float scaledValue = value * ((float) VMETER_PIXEL_RANGE / 2.0);
  if (scaledValue < -vMeterDisplayableRange) {
    scaledValue = -vMeterDisplayableRange;
  } else if (scaledValue > vMeterDisplayableRange) {
    scaledValue = vMeterDisplayableRange;
  }
  return roundf (((float) displayHeight / 2.0f) + (scaledValue - ((float) VMETER_HEIGHT / 2.0)));
}

// ------------------------------------------------------------------- runVerticalMeters

void runVerticalMeters (struct MeterData *data) {
  data->buffer->fillSprite (TFT_DARKGREY);
  
  int32_t meterOffset = meterYOffsetForMachineValue (data->source1, data->displayHeight);
  vMeterSprite->pushToSprite (data->buffer, VMETER_A_X, meterOffset);
  if (data->blinkState && data->source1Overloaded) {
    vLabel1OverSprite->pushToSprite (data->buffer, VMETER_A_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  } else {
    vLabel1Sprite->pushToSprite (data->buffer, VMETER_A_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  }
  if (data->inputEnabledCount < 2) {
    goto done;
  }

  meterOffset = meterYOffsetForMachineValue (data->source2, data->displayHeight);
  vMeterSprite->pushToSprite (data->buffer, VMETER_B_X, meterOffset);
  if (data->blinkState && data->source2Overloaded) {
    vLabel2OverSprite->pushToSprite (data->buffer, VMETER_B_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  } else {
    vLabel2Sprite->pushToSprite (data->buffer, VMETER_B_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  }
  if (data->inputEnabledCount < 3) {
    goto done;
  }

  meterOffset = meterYOffsetForMachineValue (data->source3, data->displayHeight);
  vMeterSprite->pushToSprite (data->buffer, VMETER_C_X, meterOffset);
  if (data->blinkState && data->source3Overloaded) {
    vLabel3OverSprite->pushToSprite (data->buffer, VMETER_C_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  } else {
    vLabel3Sprite->pushToSprite (data->buffer, VMETER_C_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  }
  if (data->inputEnabledCount < 4) {
    goto done;
  }
  
  meterOffset = meterYOffsetForMachineValue (data->source4, data->displayHeight);
  vMeterSprite->pushToSprite (data->buffer, VMETER_D_X, meterOffset);
  if (data->blinkState && data->source4Overloaded) {
    vLabel4OverSprite->pushToSprite (data->buffer, VMETER_D_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  } else {
    vLabel4Sprite->pushToSprite (data->buffer, VMETER_D_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  }
  if (data->inputEnabledCount < 5) {
    goto done;
  }
  
  meterOffset = meterYOffsetForMachineValue (data->source5, data->displayHeight);
  vMeterSprite->pushToSprite (data->buffer, VMETER_E_X, meterOffset);
  if (data->blinkState && data->source5Overloaded) {
    vLabel5OverSprite->pushToSprite (data->buffer, VMETER_E_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  } else {
    vLabel5Sprite->pushToSprite (data->buffer, VMETER_E_X + VMETER_LABEL_X_OFFSET, VMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  }
  
done:

  data->buffer->drawRect (0, data->displayHeight / 2, data->displayWidth, 2, TFT_RED);
}

// ------------------------------------------------------------------- createVMeterSprite

#define VMETER_WIDE_TICK 10
#define VMETER_NARROW_TICK 4

TFT_eSprite *createVMeterSprite (TFT_eSPI *tft) {
  TFT_eSprite *sprite = new TFT_eSprite (tft);
  
  // Create sprite.
  sprite = new TFT_eSprite (tft);
  sprite->setColorDepth (16);
  sprite->createSprite (VMETER_WIDTH, VMETER_HEIGHT);

  // Background of sprite.
  sprite->fillSprite (TFT_DARKGREY);
  int meterTop = (VMETER_HEIGHT - VMETER_PIXEL_RANGE) / 2;
  sprite->fillRect (0, meterTop - VMETER_V_MARGIN, VMETER_WIDTH, VMETER_PIXEL_RANGE + (2 * VMETER_V_MARGIN), TFT_WHITE);
  
  // Tick marks.
  int gap = VMETER_PIXEL_RANGE / 100;
  for (int i = 0; i <= 100; i++) {
    bool isNegative = i > 50;
    sprite->drawLine (0, meterTop + (i * gap), i % 10 == 5 ? VMETER_WIDE_TICK : VMETER_NARROW_TICK, meterTop + (i * gap), isNegative ? TFT_RED : TFT_BLACK);
    sprite->drawLine (VMETER_WIDTH - (i % 10 == 5 ? VMETER_WIDE_TICK : VMETER_NARROW_TICK), meterTop + (i * gap), VMETER_WIDTH, meterTop + (i * gap), isNegative ? TFT_RED : TFT_BLACK);
  }

  // Numbers.
  char textBuffer[6];
  sprite->setFreeFont (&FreeSans9pt7b);
  sprite->setTextDatum (MC_DATUM);
  for (int i = -5; i <= 5; i++) {
    float floatNum = (float) i / 5.0f;
    if (i == 0) {
      sprintf (textBuffer, "0");
    } else {
      sprintf (textBuffer, "%.1f", -floatNum);
    }
    bool isNegative = i > 0;
    sprite->setTextColor (isNegative ? TFT_RED : TFT_BLACK, TFT_WHITE);
    sprite->drawString (textBuffer, VMETER_WIDTH / 2, (VMETER_HEIGHT / 2) + (i * gap * 10) - 2);
  }
  sprite->setTextDatum (TL_DATUM);

  return sprite;
}

// ------------------------------------------------------------------- prepareVerticalMeters

float prepareVerticalMeters (TFT_eSPI *tft) {
  vMeterSprite = createVMeterSprite (tft);

  vLabel1Sprite = createLabelSprite (tft, "1", METER_LABEL_COLOR_1, TFT_WHITE, -2);
  vLabel2Sprite = createLabelSprite (tft, "2", METER_LABEL_COLOR_2, TFT_WHITE, 0);
  vLabel3Sprite = createLabelSprite (tft, "3", METER_LABEL_COLOR_3, TFT_WHITE, -1);
  vLabel4Sprite = createLabelSprite (tft, "4", METER_LABEL_COLOR_4, TFT_WHITE, -1);
  vLabel5Sprite = createLabelSprite (tft, "5", METER_LABEL_COLOR_5, TFT_WHITE, 0);

  vLabel1OverSprite = createLabelSprite (tft, "1", TFT_WHITE, TFT_BLACK, -2);
  vLabel2OverSprite = createLabelSprite (tft, "2", TFT_WHITE, TFT_BLACK, 0);
  vLabel3OverSprite = createLabelSprite (tft, "3", TFT_WHITE, TFT_BLACK, -1);
  vLabel4OverSprite = createLabelSprite (tft, "4", TFT_WHITE, TFT_BLACK, -1);
  vLabel5OverSprite = createLabelSprite (tft, "5", TFT_WHITE, TFT_BLACK, 0);

  return VMETER_ALPHA;
}

// ------------------------------------------------------------------- cleanupVerticalMeters

void cleanupVerticalMeters (void) {
  if (vMeterSprite) {
    vMeterSprite->deleteSprite();
    delete vMeterSprite;
    vMeterSprite = nullptr;
  }

  vLabel1Sprite = destroySprite (vLabel1Sprite);
  vLabel2Sprite = destroySprite (vLabel2Sprite);
  vLabel3Sprite = destroySprite (vLabel3Sprite);
  vLabel4Sprite = destroySprite (vLabel4Sprite);
  vLabel5Sprite = destroySprite (vLabel5Sprite);

  vLabel1OverSprite = destroySprite (vLabel1OverSprite);
  vLabel2OverSprite = destroySprite (vLabel2OverSprite);
  vLabel3OverSprite = destroySprite (vLabel3OverSprite);
  vLabel4OverSprite = destroySprite (vLabel4OverSprite);
  vLabel5OverSprite = destroySprite (vLabel5OverSprite);
}
