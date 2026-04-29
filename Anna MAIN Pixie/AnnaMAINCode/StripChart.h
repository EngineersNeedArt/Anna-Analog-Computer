// =============================================================================
//                                StripChart.cpp
// =============================================================================

#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <TFT_eSPI.h>
#include "MeterUtils.h"


#ifdef __cplusplus
extern "C" {
#endif

// Call repeatedly while metering mode is strip.
void runStripChart (struct MeterData *data);

// Call once when metering mode switched to Strip. Prepares sprites.
float prepareStripChart (TFT_eSPI *tft);

// Called to free up memory (sprites).
void cleanupStripChart (void);

#ifdef __cplusplus
}
#endif
