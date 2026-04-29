// =============================================================================
//                                 SweepMeters.h
// =============================================================================

#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <TFT_eSPI.h>
#include "MeterUtils.h"


#ifdef __cplusplus
extern "C" {
#endif


// Call repeatedly while metering mode is sweep meters.
void runSweepMeters (struct MeterData *data);

// Call once when metering mode switched to sweep meters. Prepares sprites. Return alpha.
float prepareSweepMeters (TFT_eSPI *tft);

// Called to free up memory (sprites).
void cleanupSweepMeters (void);

#ifdef __cplusplus
}
#endif