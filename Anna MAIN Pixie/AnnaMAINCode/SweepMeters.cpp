// =============================================================================
//                               SweepMeters.cpp
// =============================================================================

#include <Arduino.h>
#include "SweepMeters.h"
#include "Utilities.h"


#define SWEEPMETER_WIDTH    240
#define SWEEP_ORIGIN_Y      160

#define NEEDLE_END          148.0
#define NEEDLE_START        64.0
#define NEEDLE_WIDTH        3.0

#define SWEEPMETER_LABEL_X1 107 - 80  // 107 = (screenWidth / 2) - (LABEL_WIDTH / 2)
#define SWEEPMETER_LABEL_X2 107 - 40
#define SWEEPMETER_LABEL_X3 107 - 0
#define SWEEPMETER_LABEL_X4 107 + 40
#define SWEEPMETER_LABEL_X5 107 + 80

#define SWEEPMETER_LABEL_Y  106

#define SWEEPMETER_ALPHA     0.10f


TFT_eSprite *sweepMeterSprite = nullptr;

TFT_eSprite *sweepLabel1Sprite = nullptr;
TFT_eSprite *sweepLabel2Sprite = nullptr;
TFT_eSprite *sweepLabel3Sprite = nullptr;
TFT_eSprite *sweepLabel4Sprite = nullptr;
TFT_eSprite *sweepLabel5Sprite = nullptr;

TFT_eSprite *sweepLabel1OverSprite = nullptr;
TFT_eSprite *sweepLabel2OverSprite = nullptr;
TFT_eSprite *sweepLabel3OverSprite = nullptr;
TFT_eSprite *sweepLabel4OverSprite = nullptr;
TFT_eSprite *sweepLabel5OverSprite = nullptr;


// =================================================================== Functions
// ------------------------------------------------------------------- machineValueToAngle

#define SWEEPMETER_MAXANGLE 50.0f

// Maps value of -1.0 to +1.0 to angle of -135° to -45°

float machineValueToAngle (float value) {
  float angle = (value * 45.0);
  if (angle < -SWEEPMETER_MAXANGLE) {
    angle = -SWEEPMETER_MAXANGLE;
  } else if (angle > SWEEPMETER_MAXANGLE) {
    angle = SWEEPMETER_MAXANGLE;
  }
  
  // Linear interpolation.
  return -90 + angle;
}

// ------------------------------------------------------------------- drawNeedle

void drawNeedle (TFT_eSprite *buffer, uint16_t meterWidth, float value, uint16_t color) {
  float angleDeg = machineValueToAngle (value);
  float angleRad = angleDeg * (PI / 180.0);
  
  // Direction vector.
  float dx = cos (angleRad);
  float dy = sin (angleRad);

  // Perpendicular vector for width.
  float nx = -dy;
  float ny = dx;

  // Compute 4 corners of the needle polygon.
  float halfW = NEEDLE_WIDTH / 2.0;
  float x0 = (meterWidth / 2) + dx * NEEDLE_START + nx * halfW;  // base left
  float y0 = SWEEP_ORIGIN_Y + dy * NEEDLE_START + ny * halfW;
  float x1 = (meterWidth / 2) + dx * NEEDLE_START - nx * halfW;  // base right
  float y1 = SWEEP_ORIGIN_Y + dy * NEEDLE_START - ny * halfW;
  float x2 = (meterWidth / 2) + dx * NEEDLE_END + nx * halfW;  // tip left
  float y2 = SWEEP_ORIGIN_Y + dy * NEEDLE_END + ny * halfW;
  float x3 = (meterWidth / 2) + dx * NEEDLE_END - nx * halfW;  // tip right
  float y3 = SWEEP_ORIGIN_Y + dy * NEEDLE_END - ny * halfW;
  
  // Draw the two triangles
  buffer->fillTriangle (x0 + 1, y0, x1 + 1, y1, x2 + 1, y2, color);
  buffer->fillTriangle (x1 + 1, y1, x2 + 1, y2, x3 + 1, y3, color);
}

// ------------------------------------------------------------------- runSweepMeters

void runSweepMeters (struct MeterData *data) {
  sweepMeterSprite->pushToSprite (data->buffer, 0, 0);

  // Draw needles.
  if (data->inputEnabledCount >= 5) {
    drawNeedle (data->buffer, data->displayWidth, data->source5, METER_LABEL_COLOR_5);
  }
  if (data->inputEnabledCount >= 4) {
    drawNeedle (data->buffer, data->displayWidth, data->source4, METER_LABEL_COLOR_4);
  }
  if (data->inputEnabledCount >= 3) {
    drawNeedle (data->buffer, data->displayWidth, data->source3, METER_LABEL_COLOR_3);
  }
  if (data->inputEnabledCount >= 2) {
    drawNeedle (data->buffer, data->displayWidth, data->source2, METER_LABEL_COLOR_2);
  }
  drawNeedle (data->buffer, data->displayWidth, data->source1, METER_LABEL_COLOR_1);
  
  // Draw labels last.
  if (data->blinkState && data->source1Overloaded) {
    sweepLabel1OverSprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X1, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  } else {
    sweepLabel1Sprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X1, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  }
  if (data->inputEnabledCount < 2) {
    goto done;
  }
  
  if (data->blinkState && data->source2Overloaded) {
    sweepLabel2OverSprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X2, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  } else {
    sweepLabel2Sprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X2, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  }
  if (data->inputEnabledCount < 3) {
    goto done;
  }

  if (data->blinkState && data->source3Overloaded) {
    sweepLabel3OverSprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X3, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  } else {
    sweepLabel3Sprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X3, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  }
  if (data->inputEnabledCount < 4) {
    goto done;
  }

  if (data->blinkState && data->source4Overloaded) {
    sweepLabel4OverSprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X4, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  } else {
    sweepLabel4Sprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X4, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  }
  if (data->inputEnabledCount < 5) {
    goto done;
  }
  
  if (data->blinkState && data->source5Overloaded) {
    sweepLabel5OverSprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X5, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  } else {
    sweepLabel5Sprite->pushToSprite (data->buffer, SWEEPMETER_LABEL_X5, SWEEPMETER_LABEL_Y, TFT_TRANSPARENT);
  }
  
done:

  // Blit to display.
  data->buffer->pushSprite (0, 0);
}

// ------------------------------------------------------------------- createSweepMeterSprite

TFT_eSprite *createSweepMeterSprite (TFT_eSPI *tft) {
  TFT_eSprite *sprite = new TFT_eSprite (tft);
  uint16_t width = tft->width ();
  uint16_t height = tft->height ();

  // Create sprite.
  sprite = new TFT_eSprite (tft);
  sprite->setColorDepth (16);
  sprite->createSprite (width, height);

  // Background of sprite and white panel arc.
  sprite->fillSprite (TFT_DARKGREY);
  sprite->drawSmoothArc (width / 2, SWEEP_ORIGIN_Y, NEEDLE_END, NEEDLE_END * 2.0 / 3.0, 135, 225, TFT_WHITE, TFT_LIGHTGREY, false);

  // Tick marks.
  for (int i = -25; i <= 25; i++) {
    float angleRad = (((float) i * 1.8f) + 90.0) * (PI / 180.0);
    float dx = cos (angleRad);
    float dy = sin (angleRad);

    float length = (i % 5 == 0) ? 16.0 : 8.0;
    float x0 = (width / 2) + (dx * NEEDLE_END);
    float y0 = SWEEP_ORIGIN_Y - (dy * NEEDLE_END);
    float x1 = (width / 2) + (dx * (NEEDLE_END - length));
    float y1 = SWEEP_ORIGIN_Y - (dy * (NEEDLE_END - length));
    
    sprite->drawWideLine (x0, y0, x1, y1, 1.0, TFT_DARKGREY, TFT_WHITE);
  }

  return sprite;
}

// ------------------------------------------------------------------- prepareSweepMeters

float prepareSweepMeters (TFT_eSPI *tft) {
  sweepMeterSprite = createSweepMeterSprite (tft);

  sweepLabel1Sprite = createLabelSprite (tft, "1", METER_LABEL_COLOR_1, TFT_WHITE, -2);
  sweepLabel2Sprite = createLabelSprite (tft, "2", METER_LABEL_COLOR_2, TFT_WHITE, 0);
  sweepLabel3Sprite = createLabelSprite (tft, "3", METER_LABEL_COLOR_3, TFT_WHITE, -1);
  sweepLabel4Sprite = createLabelSprite (tft, "4", METER_LABEL_COLOR_4, TFT_WHITE, -1);
  sweepLabel5Sprite = createLabelSprite (tft, "5", METER_LABEL_COLOR_5, TFT_WHITE, 0);

  sweepLabel1OverSprite = createLabelSprite (tft, "1", TFT_WHITE, METER_LABEL_COLOR_1, -2);
  sweepLabel2OverSprite = createLabelSprite (tft, "2", TFT_WHITE, METER_LABEL_COLOR_2, 0);
  sweepLabel3OverSprite = createLabelSprite (tft, "3", TFT_WHITE, METER_LABEL_COLOR_3, -1);
  sweepLabel4OverSprite = createLabelSprite (tft, "4", TFT_WHITE, METER_LABEL_COLOR_4, -1);
  sweepLabel5OverSprite = createLabelSprite (tft, "5", TFT_WHITE, METER_LABEL_COLOR_5, 0);

  return SWEEPMETER_ALPHA;
}

// ------------------------------------------------------------------- cleanupSweepMeters

void cleanupSweepMeters (void) {
  sweepMeterSprite = destroySprite (sweepMeterSprite);

  sweepLabel1Sprite = destroySprite (sweepLabel1Sprite);
  sweepLabel2Sprite = destroySprite (sweepLabel2Sprite);
  sweepLabel3Sprite = destroySprite (sweepLabel3Sprite);
  sweepLabel4Sprite = destroySprite (sweepLabel4Sprite);
  sweepLabel5Sprite = destroySprite (sweepLabel5Sprite);

  sweepLabel1OverSprite = destroySprite (sweepLabel1OverSprite);
  sweepLabel2OverSprite = destroySprite (sweepLabel2OverSprite);
  sweepLabel3OverSprite = destroySprite (sweepLabel3OverSprite);
  sweepLabel4OverSprite = destroySprite (sweepLabel4OverSprite);
  sweepLabel5OverSprite = destroySprite (sweepLabel5OverSprite);
}
