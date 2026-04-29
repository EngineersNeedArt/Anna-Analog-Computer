// =============================================================================
//                                MassSpring.h
// =============================================================================

#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <TFT_eSPI.h>
#include "MeterUtils.h"


#ifdef __cplusplus
extern "C" {
#endif

// Call repeatedly while metering mode is mass+spring.
void runMassSpring (struct MeterData *data);

// Call once when metering mode switched to the mass+spring. Prepares sprites.
float prepareMassSpring (TFT_eSPI *tft);

// Called to free up memory (sprites).
void cleanupMassSpring (void);

#ifdef __cplusplus
}
#endif