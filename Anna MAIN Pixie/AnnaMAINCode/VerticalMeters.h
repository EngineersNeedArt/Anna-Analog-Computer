// =============================================================================
//                               VerticalMeters.h
// =============================================================================

#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <TFT_eSPI.h>
#include "MeterUtils.h"


#ifdef __cplusplus
extern "C" {
#endif

// Call repeatedly while metering mode is vertical meters.
void runVerticalMeters (struct MeterData *data);

// Call once when metering mode switched to vertical meters. Prepares sprites. Returns alpha value.
float prepareVerticalMeters (TFT_eSPI *tft);

// Called to free up memory (sprites).
void cleanupVerticalMeters (void);

#ifdef __cplusplus
}
#endif