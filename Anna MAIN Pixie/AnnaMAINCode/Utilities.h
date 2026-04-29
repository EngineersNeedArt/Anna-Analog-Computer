// =============================================================================
//                                Utilities.h
// =============================================================================

#pragma once


#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif


bool topButtonPressed (void);

bool bottomButtonPressed (void);

bool machineValueOverloaded (float value);

float clippedMachineValue (float sourceValue, bool *clipped);

void initUtilities ();

#ifdef __cplusplus
}
#endif