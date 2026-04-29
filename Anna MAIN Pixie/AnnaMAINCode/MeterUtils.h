// =============================================================================
//                                 MeterUtils.h
// =============================================================================

#pragma once


#include <stdbool.h>
#include <stdint.h>
#include <TFT_eSPI.h>


#ifdef __cplusplus
extern "C" {
#endif


#define LABEL_WIDTH         26
#define LABEL_HEIGHT        26
#define LABEL_CIRCLE_RADIUS 12

#define METER_LABEL_COLOR_1 TFT_RED
#define METER_LABEL_COLOR_2 0xFCC0  // Was TFT_ORANGE
#define METER_LABEL_COLOR_3 0x0600  // Medium Green
#define METER_LABEL_COLOR_4 TFT_BLUE
#define METER_LABEL_COLOR_5 0x6818  // Was TFT_VIOLET



struct MeterData {
    TFT_eSprite *buffer;
    uint16_t displayWidth;
    uint16_t displayHeight;
    float source1;
    float source2;
    float source3;
    float source4;
    float source5;
    bool source1Overloaded;
    bool source2Overloaded;
    bool source3Overloaded;
    bool source4Overloaded;
    bool source5Overloaded;
    int inputEnabledCount;
    bool blinkState;
    bool buttonToggle;
};

TFT_eSprite *createLabelSprite (TFT_eSPI *tft, const char *label, uint32_t fillColor, uint32_t textColor, int xBias);

TFT_eSprite *destroySprite (TFT_eSprite *sprite);

#ifdef __cplusplus
}
#endif