// =============================================================================
//                                SpriteUtils.cpp
// =============================================================================

#include <TFT_eSPI.h>
#include "SpriteUtils.h"

#define FADE_SLUGGISHNESS  254  // (0-255). Use 254 for a super-slow fade, 160 is decent. 

static uint8_t  lut6[64];
static bool didInitFade = false;

// =================================================================== Functions
// ------------------------------------------------------------------- buildFadeLUT

void buildFadeLUTs (uint8_t factor) {
    for (int i = 0; i < 64; i++) {
      lut6[i] = i * factor >> 8;
    }
    
    // Serial.println(lut6[63]);  // should print 62 — if it prints 0, LUT is broken
    // Serial.println(lut6[32]);  // should print ~31
}

// ------------------------------------------------------------------- fadeSpriteGreen

void fadeSpriteGreen (TFT_eSprite *sprite) {
    if (!didInitFade) {
      buildFadeLUTs (FADE_SLUGGISHNESS);
      didInitFade = true;
    }

    uint16_t *buf = (uint16_t *) sprite->getPointer ();
    int16_t pixelCount = sprite->width () * sprite->height ();

    for (int i = 0; i < pixelCount; i++) {
        uint16_t px = __builtin_bswap16 (buf[i]);  // swap to standard RGB565
        uint8_t g = lut6[(px >> 5) & 0x3F];
        buf[i] = __builtin_bswap16 ((px & 0xF81F) | (g << 5));  // fade G, swap back
    }
}
