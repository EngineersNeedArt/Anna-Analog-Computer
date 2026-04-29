// =============================================================================
//                                   Scope.cpp
// =============================================================================

#include <Arduino.h>
#include "Scope.h"
#include "SpriteUtils.h"


#define SCOPEMETER_ALPHA            1.00f
#define SCOPEMETER_LT_RETICLE_COLOR 0x05E0  // Light Green
#define SCOPEMETER_DK_RETICLE_COLOR 0x02E0  // Dark Green
#define SCOPEMETER_MODELABEL_X      190
#define SCOPEMETER_MODELABEL_Y      7

const int SCOPE_FAST_FADE_INTERVAL_MS = 30;
const int SCOPE_SLOW_FADE_INTERVAL_MS = 240;
const int SCOPE_GRADATION_COUNT = 5;


int32_t dotSize = 3;
uint16_t originX;
uint16_t originY;
float scale;
unsigned long lastScopeFade = 0;
int fadeInterval = SCOPE_FAST_FADE_INTERVAL_MS;
int32_t wasXPlot = 999;
int32_t wasYPlot = 999;
bool fadeSlow = true;

// =================================================================== Functions
// ------------------------------------------------------------------- runScope

void runScope (struct MeterData *data) {
  // Toggle fade mode if the top button is pressed.
  if (data->buttonToggle) {
    fadeSlow = !fadeSlow;
    dotSize = fadeSlow ? 2 : 3;
    fadeInterval = fadeSlow ? SCOPE_SLOW_FADE_INTERVAL_MS : SCOPE_FAST_FADE_INTERVAL_MS;
  }
  
  // Draw grid. Every frame (unless you can think of a better, faster way).
  for (int i = -SCOPE_GRADATION_COUNT; i <= SCOPE_GRADATION_COUNT; i++) {
    bool majorAxis = (i == 0) || (i == SCOPE_GRADATION_COUNT) || (i == -SCOPE_GRADATION_COUNT);
    uint32_t tickLength = scale;
    uint32_t color = majorAxis ? SCOPEMETER_LT_RETICLE_COLOR : SCOPEMETER_DK_RETICLE_COLOR;
    uint32_t offset = roundf ((i * scale) / float (SCOPE_GRADATION_COUNT));
    uint32_t startX = majorAxis ? roundf (originX - scale) : originX - tickLength;
    uint32_t startY = majorAxis ? roundf (originY - scale) : originY - tickLength;
    uint32_t length = majorAxis ? roundf (scale * 2) : 2 * tickLength;
    data->buffer->drawFastHLine (startX, originY + offset, length, color);
    data->buffer->drawFastVLine (originX + offset, startY, length, color);

    data->buffer->setTextFont (1);
    data->buffer->setTextSize (2);
    data->buffer->setTextDatum (TL_DATUM);
    data->buffer->setTextColor (SCOPEMETER_DK_RETICLE_COLOR, TFT_BLACK);
  }

  // Indicate mode.
  data->buffer->drawString (fadeSlow ? "SLOW" : "FAST", SCOPEMETER_MODELABEL_X, SCOPEMETER_MODELABEL_Y);
  
  unsigned long now = millis ();
  if (now > lastScopeFade) {
    fadeSpriteGreen (data->buffer);
      lastScopeFade = now + fadeInterval;
  }
  
  int32_t xPlot = originX;
  xPlot = xPlot + roundf (data->source1 * scale) - (dotSize / 2);
  int32_t yPlot = originY;
  yPlot = yPlot - roundf (data->source2 * scale) - (dotSize / 2);

  if (wasXPlot == 999) {
    // No old location, just draw.
    data->buffer->fillRect (xPlot, yPlot, dotSize, dotSize, TFT_GREEN);
  } else {
    // Claude's DDA line algorithm.
    // Draw rects between old location and new so the plot appears contiguous.
    int dx = xPlot - wasXPlot;
    int dy = yPlot - wasYPlot;
    int steps = max (abs (dx), abs (dy));
    if (steps == 0) {
        data->buffer->fillRect (xPlot, yPlot, dotSize, dotSize, TFT_GREEN);
    } else {
      float xInc = (float) dx / steps;
      float yInc = (float) dy / steps;
      float cx = wasXPlot + xInc;
      float cy = wasYPlot + yInc;
      for (int i = 1; i <= steps; i++) {
          data->buffer->fillRect ((int) roundf (cx), (int) roundf (cy), dotSize, dotSize, TFT_GREEN);
          cx += xInc;
          cy += yInc;
      }
    }
  }

  // Store away (x,y) point.
  wasXPlot = xPlot;
  wasYPlot = yPlot;
  
  data->buffer->pushSprite (0, 0);
}

// ------------------------------------------------------------------- prepareScope

float prepareScope (TFT_eSPI *tft) {
  // Sentinel value.
  wasXPlot = 999;

  // Pre-calculate origin, scale factor.
  originX = tft->width () / 2;
  originY = tft->height () / 2;
  scale = (float) min (originX, originY);
  
  // Get the largest integer multipe of 20 (10 positive and 10 negative gradations).
  scale = (floor (scale / 20.0)) * 20.0;
  
  return SCOPEMETER_ALPHA;
}

// ------------------------------------------------------------------- cleanupScope

void cleanupScope (void) {
}
