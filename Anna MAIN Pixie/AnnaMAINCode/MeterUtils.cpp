// =============================================================================
//                                MeterUtils.cpp
// =============================================================================

#include "MeterUtils.h"


// =================================================================== Functions
// ------------------------------------------------------------------- createLabelSprite

TFT_eSprite *createLabelSprite (TFT_eSPI *tft, const char *label, uint32_t fillColor, uint32_t textColor, int xBias) {
  TFT_eSprite *sprite = new TFT_eSprite (tft);

  sprite->setColorDepth (16);
  sprite->createSprite (LABEL_WIDTH, LABEL_HEIGHT);

  sprite->fillSprite (TFT_TRANSPARENT);  // Transparent.
  sprite->fillCircle (LABEL_WIDTH / 2, LABEL_HEIGHT / 2, LABEL_CIRCLE_RADIUS, fillColor);
  
  sprite->setFreeFont (&FreeSansBold9pt7b);
  sprite->setTextColor (textColor, fillColor);
  sprite->setTextDatum (MC_DATUM);
  sprite->drawString (label, (LABEL_WIDTH / 2) + xBias, (LABEL_HEIGHT / 2) - 1);

  return sprite;
}

// ------------------------------------------------------------------- destroySprite

TFT_eSprite *destroySprite (TFT_eSprite *sprite) {
  if (sprite) {
    sprite->deleteSprite();
    delete sprite;
  }

  return nullptr;
}
