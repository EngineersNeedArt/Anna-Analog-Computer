// =============================================================================
//                                LunarLander.h
// =============================================================================

#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <TFT_eSPI.h>
#include "MeterUtils.h"


#ifdef __cplusplus
extern "C" {
#endif

// Call repeatedly while metering mode is lunar lander.
void runLunarLander (struct MeterData *data);

// Call once when metering mode switched to lunar lander. Prepares sprites.
float prepareLunarLander (TFT_eSPI *tft);

// Called to free up memory (sprites).
void cleanupLunarLander (void);

#ifdef __cplusplus
}
#endif