// =============================================================================
//                                StripChart.cpp
// =============================================================================


#include <Arduino.h>
#include "StripChart.h"
#include "Utilities.h"


#define STRIP_PEN_X               239
#define STRIP_H_SCROLL            -1

#define STRIPMETER_START_Y        2
#define STRIPMETER_ADVANCE_Y      26
#define STRIPMETER_LABEL_X_OFFSET 2
#define STRIPMETER_LABEL_Y_OFFSET 2

#define STRIP_INTERVAL 1000

#define STRIPMETER_ALPHA     0.50f


TFT_eSprite *stripLabel1Sprite = nullptr;
TFT_eSprite *stripLabel2Sprite = nullptr;
TFT_eSprite *stripLabel3Sprite = nullptr;
TFT_eSprite *stripLabel4Sprite = nullptr;
TFT_eSprite *stripLabel5Sprite = nullptr;

TFT_eSprite *stripLabel1OverSprite = nullptr;
TFT_eSprite *stripLabel2OverSprite = nullptr;
TFT_eSprite *stripLabel3OverSprite = nullptr;
TFT_eSprite *stripLabel4OverSprite = nullptr;
TFT_eSprite *stripLabel5OverSprite = nullptr;

int32_t wasY1 = 0;
int32_t wasY2 = 0;
int32_t wasY3 = 0;
int32_t wasY4 = 0;
int32_t wasY5 = 0;

unsigned long nextStrip = 0;

bool startingStrip = false;


// =================================================================== Functions
// ------------------------------------------------------------------- runStripChart

void runStripChart (struct MeterData *data) {
  int32_t y;

  // Draw initial rules.
  if (startingStrip) {
    for (int i = 0; i < 9; i++) {
      y = roundf ((i + 1) * (data->displayHeight / 10.0));
      data->buffer->drawFastHLine (0, y, data->displayWidth, i == 4 ? TFT_LIGHTGREY : TFT_DARKGREY);
    }
  }

  data->buffer->scroll (STRIP_H_SCROLL, 0);

  // Draw scrolling rules.
  for (int i = 0; i < 9; i++) {
    y = roundf ((i + 1) * (data->displayHeight / 10.0));
    data->buffer->drawFastHLine (STRIP_PEN_X + STRIP_H_SCROLL, y, -STRIP_H_SCROLL, i == 4 ? TFT_LIGHTGREY : TFT_DARKGREY);
  }

  // Occassional vertical rule.
  unsigned long now = millis ();
  if (now >= nextStrip) {
    data->buffer->drawFastVLine (STRIP_PEN_X + STRIP_H_SCROLL, 0, data->displayHeight, TFT_DARKGREY);
    nextStrip = now + STRIP_INTERVAL;
  }

  // Draw data source lines.
  y = roundf ((1.0 - clippedMachineValue (data->source5, NULL)) * (data->displayHeight / 2));
  if (startingStrip) {
    wasY5 = y;
  }
  if (data->inputEnabledCount >= 5) {
    data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY5, STRIP_PEN_X, y, METER_LABEL_COLOR_5);
    data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY5 + 1, STRIP_PEN_X, y + 1, METER_LABEL_COLOR_5);
  }
  wasY5 = y;
  
  y = roundf ((1.0 - clippedMachineValue (data->source4, NULL)) * (data->displayHeight / 2));
  if (startingStrip) {
    wasY4 = y;
  }
  if (data->inputEnabledCount >= 4) {
    data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY4, STRIP_PEN_X, y, METER_LABEL_COLOR_4);
    data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY4 + 1, STRIP_PEN_X, y + 1, METER_LABEL_COLOR_4);
  }
  wasY4 = y;

  y = roundf ((1.0 - clippedMachineValue (data->source3, NULL)) * (data->displayHeight / 2));
  if (startingStrip) {
    wasY3 = y;
  }
  if (data->inputEnabledCount >= 3) {
    data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY3, STRIP_PEN_X, y, METER_LABEL_COLOR_3);
    data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY3 + 1, STRIP_PEN_X, y + 1, METER_LABEL_COLOR_3);
  }
  wasY3 = y;

  y = roundf ((1.0 - clippedMachineValue (data->source2, NULL)) * (data->displayHeight / 2));
  if (startingStrip) {
    wasY2 = y;
  }
  if (data->inputEnabledCount >= 2) {
    data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY2, STRIP_PEN_X, y, METER_LABEL_COLOR_2);
    data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY2 + 1, STRIP_PEN_X, y + 1, METER_LABEL_COLOR_2);
  }
  wasY2 = y;
  
  y = roundf ((1.0 - clippedMachineValue (data->source1, NULL)) * (data->displayHeight / 2));
  if (startingStrip) {
    wasY1 = y;
  }
  data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY1, STRIP_PEN_X, y, METER_LABEL_COLOR_1);
  data->buffer->drawLine (STRIP_PEN_X + STRIP_H_SCROLL, wasY1 + 1, STRIP_PEN_X, y + 1, METER_LABEL_COLOR_1);
  wasY1 = y;
  
  data->buffer->fillRect (0, 0, LABEL_WIDTH, data->displayHeight, TFT_BLACK);

  int16_t yPos = STRIPMETER_START_Y;
  if (data->blinkState && data->source1Overloaded) {
    stripLabel1OverSprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  } else {
    stripLabel1Sprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
  }
  yPos = yPos + STRIPMETER_ADVANCE_Y;

  if (data->inputEnabledCount >= 2) {
    if (data->blinkState && data->source2Overloaded) {
      stripLabel2OverSprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
    } else {
      stripLabel2Sprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);    
    }
  }
  yPos = yPos + STRIPMETER_ADVANCE_Y;

  if (data->inputEnabledCount >= 3) {
    if (data->blinkState && data->source3Overloaded) {
      stripLabel3OverSprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
    } else {
      stripLabel3Sprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
    }
  }
  yPos = yPos + STRIPMETER_ADVANCE_Y;

  if (data->inputEnabledCount >= 4) {
    if (data->blinkState && data->source4Overloaded) {
      stripLabel4OverSprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
    } else {
      stripLabel4Sprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
    }
  }
  yPos = yPos + STRIPMETER_ADVANCE_Y;

  if (data->inputEnabledCount >= 5) {
    if (data->blinkState && data->source5Overloaded) {
      stripLabel5OverSprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
    } else {
      stripLabel5Sprite->pushToSprite (data->buffer, STRIPMETER_LABEL_X_OFFSET, yPos + STRIPMETER_LABEL_Y_OFFSET, TFT_TRANSPARENT);
    }
  }
  data->buffer->pushSprite (0, 0);
  
  startingStrip = false;
}

// ------------------------------------------------------------------- prepareStripChart

float prepareStripChart (TFT_eSPI *tft) {
  stripLabel1Sprite = createLabelSprite (tft, "1", TFT_RED, TFT_WHITE, -2);
  stripLabel2Sprite = createLabelSprite (tft, "2", TFT_ORANGE, TFT_WHITE, 0);
  stripLabel3Sprite = createLabelSprite (tft, "3", METER_LABEL_COLOR_3, TFT_WHITE, -1);
  stripLabel4Sprite = createLabelSprite (tft, "4", TFT_BLUE, TFT_WHITE, -1);
  stripLabel5Sprite = createLabelSprite (tft, "5", TFT_VIOLET, TFT_WHITE, 0);

  stripLabel1OverSprite = createLabelSprite (tft, "1", TFT_WHITE, TFT_RED, -2);
  stripLabel2OverSprite = createLabelSprite (tft, "2", TFT_WHITE, TFT_ORANGE, 0);
  stripLabel3OverSprite = createLabelSprite (tft, "3", TFT_WHITE, METER_LABEL_COLOR_3, -1);
  stripLabel4OverSprite = createLabelSprite (tft, "4", TFT_WHITE, TFT_BLUE, -1);
  stripLabel5OverSprite = createLabelSprite (tft, "5", TFT_WHITE, TFT_VIOLET, 0);

  wasY1 = 0;
  wasY2 = 0;
  wasY3 = 0;
  wasY4 = 0;
  wasY5 = 0;

  startingStrip = true;

  return STRIPMETER_ALPHA;
}

// ------------------------------------------------------------------- cleanupStripChart

void cleanupStripChart (void) {
  stripLabel1Sprite = destroySprite (stripLabel1Sprite);
  stripLabel2Sprite = destroySprite (stripLabel2Sprite);
  stripLabel3Sprite = destroySprite (stripLabel3Sprite);
  stripLabel4Sprite = destroySprite (stripLabel4Sprite);
  stripLabel5Sprite = destroySprite (stripLabel5Sprite);

  stripLabel1OverSprite = destroySprite (stripLabel1OverSprite);
  stripLabel2OverSprite = destroySprite (stripLabel2OverSprite);
  stripLabel3OverSprite = destroySprite (stripLabel3OverSprite);
  stripLabel4OverSprite = destroySprite (stripLabel4OverSprite);
  stripLabel5OverSprite = destroySprite (stripLabel5OverSprite);
}
