// =============================================================================
//                                RawDisplay.h
// =============================================================================

#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <TFT_eSPI.h>
#include "MeterUtils.h"


#ifdef __cplusplus
extern "C" {
#endif


struct RawData {
    float source1;
    float source2;
    float source3;
    float source4;
    float source5;
    float rawUnitOne;
    float rawUnitZero;
    float rawUnitNegOne;
};

// Call repeatedly while metering mode is raw display.
void runRawDisplay (struct MeterData *meterData, struct RawData *rawData);

// Call once when metering mode switched to the raw display.
float prepareRawDisplay (TFT_eSPI *tft);

// Called to free up memory.
void cleanupRawDisplay (void);

#ifdef __cplusplus
}
#endif