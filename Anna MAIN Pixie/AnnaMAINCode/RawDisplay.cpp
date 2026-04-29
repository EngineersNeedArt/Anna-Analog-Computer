// =============================================================================
//                                 RawDisplay.cpp
// =============================================================================

#include <Arduino.h>
#include "RawDisplay.h"


#define RAW_DISPLAY_ROW_TOP 28
#define RAW_DISPLAY_ROW_HEIGHT 20
#define RAW_DISPLAY_LEFT_COLUMN1 48
#define RAW_DISPLAY_RIGHT_COLUMN1 52
#define RAW_DISPLAY_LEFT_COLUMN2 178
#define RAW_DISPLAY_RIGHT_COLUMN2 182

#define RAW_DISPLAY_ALPHA     1.00f

// =================================================================== Functions
// ------------------------------------------------------------------- runRawDisplay

void runRawDisplay (struct MeterData *meterData, struct RawData *rawData) {
  char stringBuffer[8];
  int yCursor = RAW_DISPLAY_ROW_TOP;

  // Black sky.
  meterData->buffer->fillSprite (TFT_BLACK);

  meterData->buffer->setTextFont (1);
  meterData->buffer->setTextSize (2);

  meterData->buffer->setTextDatum (TC_DATUM);
  meterData->buffer->setTextColor (TFT_LIGHTGREY, TFT_BLACK);
  meterData->buffer->drawString ("Raw Values", meterData->displayWidth / 2, 2);
  meterData->buffer->drawFastHLine (0, 20, meterData->displayWidth, TFT_LIGHTGREY);

  meterData->buffer->setTextDatum (TR_DATUM);
  meterData->buffer->setTextColor (meterData->blinkState && meterData->source1Overloaded ? TFT_WHITE : METER_LABEL_COLOR_1, TFT_BLACK);
  meterData->buffer->drawString ("SRC1", RAW_DISPLAY_LEFT_COLUMN1, yCursor);
  meterData->buffer->setTextDatum (TL_DATUM);
  meterData->buffer->setTextColor (TFT_WHITE, TFT_BLACK);
  dtostrf (rawData->source1, 0, 0, stringBuffer);
  meterData->buffer->drawString (stringBuffer, RAW_DISPLAY_RIGHT_COLUMN1, yCursor);
  yCursor += RAW_DISPLAY_ROW_HEIGHT;

  meterData->buffer->setTextDatum (TR_DATUM);
  meterData->buffer->setTextColor (meterData->blinkState && meterData->source2Overloaded ? TFT_WHITE : METER_LABEL_COLOR_2, TFT_BLACK);
  meterData->buffer->drawString ("SRC2", RAW_DISPLAY_LEFT_COLUMN1, yCursor);
  meterData->buffer->setTextDatum (TL_DATUM);
  meterData->buffer->setTextColor (TFT_WHITE, TFT_BLACK);
  dtostrf (rawData->source2, 0, 0, stringBuffer);
  meterData->buffer->drawString (stringBuffer, RAW_DISPLAY_RIGHT_COLUMN1, yCursor);
  yCursor += RAW_DISPLAY_ROW_HEIGHT;

  meterData->buffer->setTextDatum (TR_DATUM);
  meterData->buffer->setTextColor (meterData->blinkState && meterData->source3Overloaded ? TFT_WHITE : METER_LABEL_COLOR_3, TFT_BLACK);
  meterData->buffer->drawString ("SRC3", RAW_DISPLAY_LEFT_COLUMN1, yCursor);
  meterData->buffer->setTextDatum (TL_DATUM);
  meterData->buffer->setTextColor (TFT_WHITE, TFT_BLACK);
  dtostrf (rawData->source3, 0, 0, stringBuffer);
  meterData->buffer->drawString (stringBuffer, RAW_DISPLAY_RIGHT_COLUMN1, yCursor);
  yCursor += RAW_DISPLAY_ROW_HEIGHT;

  meterData->buffer->setTextDatum (TR_DATUM);
  meterData->buffer->setTextColor (meterData->blinkState && meterData->source4Overloaded ? TFT_WHITE : METER_LABEL_COLOR_4, TFT_BLACK);
  meterData->buffer->drawString ("SRC4", RAW_DISPLAY_LEFT_COLUMN1, yCursor);
  meterData->buffer->setTextDatum (TL_DATUM);
  meterData->buffer->setTextColor (TFT_WHITE, TFT_BLACK);
  dtostrf (rawData->source4, 0, 0, stringBuffer);
  meterData->buffer->drawString (stringBuffer, RAW_DISPLAY_RIGHT_COLUMN1, yCursor);
  
  yCursor = RAW_DISPLAY_ROW_TOP;
  meterData->buffer->setTextDatum (TR_DATUM);
  meterData->buffer->setTextColor (meterData->blinkState && meterData->source5Overloaded ? TFT_WHITE : METER_LABEL_COLOR_5, TFT_BLACK);
  meterData->buffer->drawString ("SRC5", RAW_DISPLAY_LEFT_COLUMN2, yCursor);
  meterData->buffer->setTextDatum (TL_DATUM);
  meterData->buffer->setTextColor (TFT_WHITE, TFT_BLACK);
  dtostrf (rawData->source5, 0, 0, stringBuffer);
  meterData->buffer->drawString (stringBuffer, RAW_DISPLAY_RIGHT_COLUMN2, yCursor);
  yCursor += RAW_DISPLAY_ROW_HEIGHT;
  
  meterData->buffer->setTextDatum (TR_DATUM);
  meterData->buffer->setTextColor (TFT_DARKGREY, TFT_BLACK);
  meterData->buffer->drawString ("+UNIT", RAW_DISPLAY_LEFT_COLUMN2, yCursor);
  meterData->buffer->setTextDatum (TL_DATUM);
  meterData->buffer->setTextColor (TFT_WHITE, TFT_BLACK);
  dtostrf (rawData->rawUnitOne, 0, 0, stringBuffer);
  meterData->buffer->drawString (stringBuffer, RAW_DISPLAY_RIGHT_COLUMN2, yCursor);
  yCursor += RAW_DISPLAY_ROW_HEIGHT;

  meterData->buffer->setTextDatum (TR_DATUM);
  meterData->buffer->setTextColor (TFT_DARKGREY, TFT_BLACK);
  meterData->buffer->drawString ("ZERO", RAW_DISPLAY_LEFT_COLUMN2, yCursor);
  meterData->buffer->setTextDatum (TL_DATUM);
  meterData->buffer->setTextColor (TFT_WHITE, TFT_BLACK);
  dtostrf (rawData->rawUnitZero, 0, 0, stringBuffer);
  meterData->buffer->drawString (stringBuffer, RAW_DISPLAY_RIGHT_COLUMN2, yCursor);
  yCursor += RAW_DISPLAY_ROW_HEIGHT;

  meterData->buffer->setTextDatum (TR_DATUM);
  meterData->buffer->setTextColor (TFT_DARKGREY, TFT_BLACK);
  meterData->buffer->drawString ("-UNIT", RAW_DISPLAY_LEFT_COLUMN2, yCursor);
  meterData->buffer->setTextDatum (TL_DATUM);
  meterData->buffer->setTextColor (TFT_WHITE, TFT_BLACK);
  dtostrf (rawData->rawUnitNegOne, 0, 0, stringBuffer);
  meterData->buffer->drawString (stringBuffer, RAW_DISPLAY_RIGHT_COLUMN2, yCursor);
  yCursor += RAW_DISPLAY_ROW_HEIGHT;

  meterData->buffer->pushSprite (0, 0);
}

// ------------------------------------------------------------------- prepareRawDisplay

float prepareRawDisplay (TFT_eSPI *tft) {
  return RAW_DISPLAY_ALPHA;
}

// ------------------------------------------------------------------- cleanupRawDisplay

void cleanupRawDisplay (void) {

}

