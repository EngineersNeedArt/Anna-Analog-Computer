// =============================================================================
//                                   Scope.cpp
// =============================================================================

#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <TFT_eSPI.h>
#include "MeterUtils.h"


#ifdef __cplusplus
extern "C" {
#endif


// Call repeatedly while metering mode is scope.
void runScope (struct MeterData *data);

// Call once when metering mode switched to scope. Prepares sprites. Return alpha.
float prepareScope (TFT_eSPI *tft);

// Called to free up memory (sprites).
void cleanupScope (void);

#ifdef __cplusplus
}
#endif