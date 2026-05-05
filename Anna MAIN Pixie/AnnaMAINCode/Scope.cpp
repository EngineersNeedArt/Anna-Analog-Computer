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
#define SCOPEMETER_MODELABEL_Y1      7
#define SCOPEMETER_MODELABEL_Y2      24

const int SCOPE_FAST_FADE_INTERVAL_MS = 30;
const int SCOPE_SLOW_FADE_INTERVAL_MS = 600;  // 240, 900
const int SCOPE_GRADATION_COUNT = 5;


int32_t dotSize = 3;
uint16_t originX;
uint16_t originY;
float scale;
unsigned long lastScopeFade = 0;
int fadeInterval = SCOPE_FAST_FADE_INTERVAL_MS;
int32_t wasXPlot = 999;
int32_t wasYPlot = 999;
int16_t scopeMode = 0;
bool fadeSlow = false;
bool firstQuadrant = true;

// =================================================================== Functions
// ------------------------------------------------------------------- runScope

void _handleScopeModeChange (uint16_t displayWidth, uint16_t displayHeight) {
  fadeSlow = !fadeSlow;
  dotSize = fadeSlow ? 2 : 3;
  fadeInterval = fadeSlow ? SCOPE_SLOW_FADE_INTERVAL_MS : SCOPE_FAST_FADE_INTERVAL_MS;
  firstQuadrant = (scopeMode >= 2);

  // Calculate origin, scale factor.
  if (firstQuadrant) {
    scale = (float) min (displayWidth - 1, displayHeight - 1);  // 135, 240
    scale = (floor (scale / 10.0)) * 10.0;  // The largest integer multipe of 10 (10 gradations).
    originX = floor ((displayWidth - scale) / 2.0);
    originY = displayHeight - floor ((displayHeight - scale) / 2.0);
  } else {
    originX = displayWidth / 2;
    originY = displayHeight / 2;
    scale = (float) min (originX, originY);
    scale = (floor (scale / 20.0)) * 20.0;  // Largest integer multipe of 20 (10 + and 10 - gradations).
  }
}

// ------------------------------------------------------------------- runScope

void runScope (struct MeterData *data) {
  // Cycle fade mode if the top button pressed.
  if (data->buttonToggle) {
    bool wasQuadrant = firstQuadrant;
    scopeMode = scopeMode + 1;
    if (scopeMode >= 4) {
      scopeMode = 0;
    }
    // Set up scaling variables.
    _handleScopeModeChange (data->displayWidth, data->displayHeight);

    // If we changed quadrant mode, clear screen, disregard was-values.
    if (firstQuadrant != wasQuadrant) {
      data->buffer->fillSprite (TFT_BLACK);
      wasXPlot = 999;
    }
  }
  
  // Draw grid. Every frame (unless you can think of a better, faster way).
  if (firstQuadrant) {
    for (int i = 0; i <= SCOPE_GRADATION_COUNT; i++) {
      bool majorAxis = (i == 0);
      uint32_t color = majorAxis ? SCOPEMETER_LT_RETICLE_COLOR : SCOPEMETER_DK_RETICLE_COLOR;
      uint32_t offset = roundf ((i * scale) / float (SCOPE_GRADATION_COUNT));
      uint32_t startX = originX;
      uint32_t startY = originY - scale;
      uint32_t length = scale;
      data->buffer->drawFastHLine (startX, originY - offset, length, color);
      data->buffer->drawFastVLine (originX + offset, startY, length, color);
    }
  } else {
    for (int i = -SCOPE_GRADATION_COUNT; i <= SCOPE_GRADATION_COUNT; i++) {
      bool majorAxis = (i == 0);
      uint32_t tickLength = scale;
      uint32_t color = majorAxis ? SCOPEMETER_LT_RETICLE_COLOR : SCOPEMETER_DK_RETICLE_COLOR;
      uint32_t offset = roundf ((i * scale) / float (SCOPE_GRADATION_COUNT));
      uint32_t startX = majorAxis ? roundf (originX - scale) : originX - tickLength;
      uint32_t startY = majorAxis ? roundf (originY - scale) : originY - tickLength;
      uint32_t length = majorAxis ? roundf (scale * 2) : 2 * tickLength;
      data->buffer->drawFastHLine (startX, originY + offset, length, color);
      data->buffer->drawFastVLine (originX + offset, startY, length, color);
    }
  }
  
  // Indicate mode.
  data->buffer->setTextFont (1);
  data->buffer->setTextSize (2);
  data->buffer->setTextDatum (TL_DATUM);
  data->buffer->setTextColor (SCOPEMETER_DK_RETICLE_COLOR, TFT_BLACK);
  data->buffer->drawString (fadeSlow ? "SLOW" : "FAST", SCOPEMETER_MODELABEL_X, SCOPEMETER_MODELABEL_Y1);
  data->buffer->drawString (firstQuadrant ? "QUAD" : "FULL", SCOPEMETER_MODELABEL_X, SCOPEMETER_MODELABEL_Y2);
  
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
}

// ------------------------------------------------------------------- prepareScope

float prepareScope (TFT_eSPI *tft) {
  // Sentinel value.
  wasXPlot = 999;

  // Set up.
  _handleScopeModeChange (tft->width (), tft->height ());

  return SCOPEMETER_ALPHA;
}

// ------------------------------------------------------------------- cleanupScope

void cleanupScope (void) {
}
