// =============================================================================
//                                MassSpring.cpp
// =============================================================================

#include <Arduino.h>
#include "MassSpring.h"


#define MASSSPRING_X          120
#define MASS_WIDTH            60
#define MASS_HEIGHT           25
#define MIN_SPRING_HEIGHT     12
#define SPRING_WIDTH          24

#define MASS_COLOR            TFT_CYAN   // TFT_BLUE
#define SPRING_COLOR          TFT_YELLOW // TFT_BROWN
#define MASS_HISTORY_COLOR    TFT_BLUE

#define MASSY_HISTORY_LENGTH  90

#define MASSSPRING_INTERVAL   1000

#define MASSSPRING_ALPHA     0.50f


int massYHistory[MASSY_HISTORY_LENGTH];
int massYHistoryIndex = 0;
unsigned long nextMassSpring = 0;
bool evenOdd = false;

// =================================================================== Functions
// ------------------------------------------------------------------- runMassSpring

void runMassSpring (struct MeterData *data) {
  // Black sky.
  data->buffer->fillSprite (TFT_BLACK);

  float transformedPosition = (data->source1 + 1.0) / 2.0;
  if (transformedPosition < 0.0) {
    transformedPosition = 0.0;
  } else if (transformedPosition > 1.0) {
    transformedPosition = 1.0;
  }

  float massY = ((data->displayHeight - (MIN_SPRING_HEIGHT + MASS_HEIGHT)) * transformedPosition) + MIN_SPRING_HEIGHT;
  uint32_t massTop = roundf (massY);
  data->buffer->drawRect (MASSSPRING_X - (MASS_WIDTH / 2), massTop, MASS_WIDTH, MASS_HEIGHT, MASS_COLOR);
  
  uint32_t wasSpringY = 0;
  uint32_t x1 = MASSSPRING_X - (SPRING_WIDTH / 2);
  uint32_t x2 = MASSSPRING_X + (SPRING_WIDTH / 2);
  for (int i = 1; i < 9; i++) {
    uint32_t y = roundf ((float) i * (massY / 8.0));
    if (i == 1) {
      data->buffer->drawLine (MASSSPRING_X, 0, MASSSPRING_X, y / 2, SPRING_COLOR);
      data->buffer->drawLine (MASSSPRING_X, y / 2, x2, y, SPRING_COLOR);
    } else if (i == 8) {
      data->buffer->drawLine (x2, wasSpringY, MASSSPRING_X, (wasSpringY + y) / 2, SPRING_COLOR);
      data->buffer->drawLine (MASSSPRING_X, (wasSpringY + y) / 2, MASSSPRING_X, y, SPRING_COLOR);
    } else if (i > 1) {
      if (i % 2 == 0) {
        data->buffer->drawLine (x2, wasSpringY, x1, y, SPRING_COLOR);
      } else {
        data->buffer->drawLine (x1, wasSpringY, x2, y, SPRING_COLOR);
      }
    }
    wasSpringY = y;
  }
  
  int index = massYHistoryIndex;
  uint32_t x = MASSSPRING_X - (MASS_WIDTH / 2);
  uint32_t xScale = x / MASSY_HISTORY_LENGTH;
  for (int i = 0; i < MASSY_HISTORY_LENGTH; i++) {
    index = index - 1;
    if (index < 0) {
      index = MASSY_HISTORY_LENGTH - 1;
    }
    x = x - xScale;
    
    int historyY = massYHistory[index];
    if (historyY == 0) {
      break;
    }
    if (historyY < 0) {
      data->buffer->drawFastVLine (x, -historyY, MASS_HEIGHT, TFT_BLUE);
    } else {
      data->buffer->drawFastVLine (x, historyY, MASS_HEIGHT, TFT_NAVY);
    }
  }
  
  // Store position of mass in history buffer.
  // Alternate colors (indicate with negative value).
  unsigned long now = millis ();

  if (now >= nextMassSpring) {
    evenOdd = !evenOdd;
    nextMassSpring = now + MASSSPRING_INTERVAL;
  }
  massYHistory[massYHistoryIndex] = evenOdd ? massTop : -massTop;

  massYHistoryIndex = massYHistoryIndex + 1;
  if (massYHistoryIndex >= MASSY_HISTORY_LENGTH) {
    massYHistoryIndex = 0;
  }
}

// ------------------------------------------------------------------- prepareMassSpring

float prepareMassSpring (TFT_eSPI *tft) {
  massYHistoryIndex = 0;
  for (int i = 0; i < MASSY_HISTORY_LENGTH; i++) {
    massYHistory[i] = 0;
  }
  
  return MASSSPRING_ALPHA;
}

// ------------------------------------------------------------------- cleanupMassSpring

void cleanupMassSpring () {
  // NOP
}
