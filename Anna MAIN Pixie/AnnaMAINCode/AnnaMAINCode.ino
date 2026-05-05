// =============================================================================
//                                AnnaMAINPixie
// =============================================================================

#include <Adafruit_ADS1X15.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <Wire.h>
#include "LunarLander.h"
#include "MassSpring.h"
#include "MeterUtils.h"
#include "RawDisplay.h"
#include "Scope.h"
#include "StripChart.h"
#include "SweepMeters.h"
#include "Utilities.h"
#include "VerticalMeters.h"


#define VERTICAL_METERINGMODE     0
#define SWEEP_METERINGMODE        1
#define STRIPCHART_METERINGMODE   2
#define SCOPE_METERINGMODE        3
#define MASSSPRING_METERINGMODE   4
#define LUNARLANDER_METERINGMODE  5
#define RAWDISPLAY_METERINGMODE   6
#define METERINGMODE_COUNT        7

#define SDA_PIN                   21    // For I2C communication with external ADCs. 
#define SCL_PIN                   22    // For I2C communication with external ADCs. 
#define OVERLOAD_OUT_PIN          25    // Set high if any source is over/under voltage.
#define EXP_DAC_PIN               26    // Unused.

#define DEFAULT_ALPHA             1.0f  // 1.0 —> no alpha (averaging).


const float CLIP_EPS = 1e-6;            // Avoid divide-by-zero

Adafruit_ADS1015 adsSourcesGND;         // ADS1x15 with address==GND
Adafruit_ADS1015 adsSources3V3;         // ADS1x15 with address==3V3

TFT_eSPI tft = TFT_eSPI ();
TFT_eSprite spriteBuffer = TFT_eSprite (&tft);

uint16_t displayWidth = 0;              // 240 on T-Display.
uint16_t displayHeight = 0;             // 135 on T-Display.

float alpha = DEFAULT_ALPHA;            // Each metering mode declares alpha.

MeterData meterData;                    // Struct containing data to be passed to the meters.
RawData rawData;                        // Struct containing raw values from DACs with alpha applied.
volatile int16_t adcResults[8] = {0};   // Array of raw values read from DACs.
portMUX_TYPE adcMux = portMUX_INITIALIZER_UNLOCKED;

int meterMode = SWEEP_METERINGMODE;
uint32_t loopCount = 0;
uint32_t nextBlink = 0;
bool blinkState = false;
bool countingDown = true;
unsigned long lastTime = 0;
int overlayMode = -1;
int overlayPosition = 0;
unsigned long overlayTime = 0;

// =================================================================== Functions
// ------------------------------------------------------------------- adcTask

void adcTask (void *parameter) {
  int16_t rawUnitNegOne;
  int16_t rawUnitZero;
  int16_t rawUnitOne;
  uint8_t loopCount = 0;

  // Runs on Core 0, continuously reading ADCs.

  while (true) {
    // Read reference voltages less frequently to save cycles.
    if (loopCount % 10 == 0) {
      rawUnitNegOne = adsSources3V3.readADC_SingleEnded (0);
      rawUnitZero = adsSources3V3.readADC_SingleEnded (3);
      rawUnitOne = adsSources3V3.readADC_SingleEnded (2);
    }

    // Always read dynamic ADCs.
    int16_t source1 = adsSources3V3.readADC_SingleEnded (1);
    int16_t source2 = adsSourcesGND.readADC_SingleEnded (1);  
    int16_t source3 = adsSourcesGND.readADC_SingleEnded (0);
    int16_t source4 = adsSourcesGND.readADC_SingleEnded (3);
    int16_t source5 = adsSourcesGND.readADC_SingleEnded (2);

    portENTER_CRITICAL (&adcMux);
    adcResults[0] = source1;
    adcResults[1] = source2;
    adcResults[2] = source3;
    adcResults[3] = source4;
    adcResults[4] = source5;
    adcResults[5] = rawUnitNegOne;
    adcResults[6] = rawUnitZero;
    adcResults[7] = rawUnitOne;
    portEXIT_CRITICAL (&adcMux);

    loopCount = loopCount + 1;
  }
}

// ------------------------------------------------------------------- computeMachineFromRaw

float computeMachineFromRaw (float raw) {
  float positiveRange = rawData.rawUnitOne - rawData.rawUnitZero;
  float negativeRange = rawData.rawUnitZero - rawData.rawUnitNegOne;

  // Protect against divide by zero; fallback to assuming full-scale (rawMin=0)
  if (positiveRange < CLIP_EPS)
    positiveRange = 1.0f;
  if (negativeRange < CLIP_EPS)
    negativeRange = rawData.rawUnitZero > 0.0 ? rawData.rawUnitZero : 1.0f;
  
  float m;
  if (raw >= rawData.rawUnitZero) {
    m = (raw - rawData.rawUnitZero) / positiveRange;    // 0..1
  } else {
    m = (raw - rawData.rawUnitZero) / negativeRange;    // -1..0
  }
  
  return m;
}

// ------------------------------------------------------------------- assignMeterMode

void assignMeterMode (int newMode) {
  // Free up storage from all metering modes (will be a NOP for those already cleaned up).
  cleanupVerticalMeters ();
  cleanupSweepMeters ();
  cleanupStripChart ();
  cleanupScope ();
  cleanupMassSpring ();
  cleanupLunarLander ();
  cleanupRawDisplay ();

  // Clear buffer.
  spriteBuffer.fillSprite (TFT_BLACK);
  
  // Assign, init.
  meterMode = newMode;
  switch (meterMode) {
    case VERTICAL_METERINGMODE:
    alpha = prepareVerticalMeters (&tft);
    break;

    case SWEEP_METERINGMODE:
    alpha = prepareSweepMeters (&tft);
    break;
    
    case STRIPCHART_METERINGMODE:
    alpha = prepareStripChart (&tft);
    break;
    
    case SCOPE_METERINGMODE:
    alpha = prepareScope (&tft);
    break;
    
    case MASSSPRING_METERINGMODE:
    alpha = prepareMassSpring (&tft);
    break;

    case LUNARLANDER_METERINGMODE:
    alpha = prepareLunarLander (&tft);
    break;

    default:
    alpha = prepareRawDisplay (&tft);
    break;
  }
}

// ------------------------------------------------------------------- meteringModeSupportsDisablingInputs

bool meteringModeSupportsDisablingInputs (int mode) {
  return (mode == VERTICAL_METERINGMODE) || (mode == SWEEP_METERINGMODE) || (mode == STRIPCHART_METERINGMODE);
}

// ------------------------------------------------------------------- handleMetering

#define BLINK_INTERVAL    200

void handleMetering () {
  unsigned long now = millis ();
  if (now >= nextBlink) {
    blinkState = !blinkState;
    nextBlink = now + BLINK_INTERVAL;
    meterData.blinkState = blinkState;
  }
  
  // Assign to meter data.
  meterData.source1 = computeMachineFromRaw (rawData.source1);
  meterData.source2 = computeMachineFromRaw (rawData.source2);
  meterData.source3 = computeMachineFromRaw (rawData.source3);
  meterData.source4 = computeMachineFromRaw (rawData.source4);
  meterData.source5 = computeMachineFromRaw (rawData.source5);
  
  switch (meterMode) {
    case VERTICAL_METERINGMODE:
    runVerticalMeters (&meterData);
    break;

    case SWEEP_METERINGMODE:
    runSweepMeters (&meterData);
    break;
    
    case STRIPCHART_METERINGMODE:
    runStripChart (&meterData);
    break;

    case SCOPE_METERINGMODE:
    runScope (&meterData);
    break;

    case MASSSPRING_METERINGMODE:
    runMassSpring (&meterData);
    break;

    case LUNARLANDER_METERINGMODE:
    runLunarLander (&meterData);
    break;

    default:
    runRawDisplay (&meterData, &rawData);
    break;
  }

  if (bottomButtonPressed ()) {
    meterMode = meterMode + 1;
    if (meterMode >= METERINGMODE_COUNT) {
      meterMode = 0;  // Wrap around to beginning metering mode.
    }
    assignMeterMode (meterMode);

    overlayMode = 0;
    overlayPosition = displayHeight;
  }
}

char *_meteringModeDisplayName () {
  switch (meterMode) {
    case VERTICAL_METERINGMODE:
    return "Vertical Meters";
    break;

    case SWEEP_METERINGMODE:
    return "Sweep Meters";
    break;
    
    case STRIPCHART_METERINGMODE:
    return "Strip Chart";
    break;
    
    case SCOPE_METERINGMODE:
    return "XY 'Scope";
    break;

    case MASSSPRING_METERINGMODE:
    return "Spring + Mass";
    break;
    
    case LUNARLANDER_METERINGMODE:
    return "Lunar Lander";
    break;

    default:
    return "Raw Values";
    break;
  }
}

void handleOverlay (void) {
  if (overlayMode < 0) {
    return;
  }
  
  if (overlayMode == 0) {
    overlayPosition = overlayPosition - 1;
    if (overlayPosition <= displayHeight - 21) {
      overlayMode = 1;
      overlayTime = millis () + 1000;
    }
  } else if (overlayMode == 1) {
      if (millis () > overlayTime) {
        overlayMode = 2;
      }
  } else if (overlayMode == 2) {
    overlayPosition = overlayPosition + 1;
    if (overlayPosition > displayHeight) {
      overlayMode = -1;
      return;
    }
  }

  spriteBuffer.drawFastHLine (0, overlayPosition - 1, displayWidth, TFT_BLACK);
  spriteBuffer.fillRect (0, overlayPosition, displayWidth, 21, TFT_WHITE);
  spriteBuffer.fillRect (0, overlayPosition, displayWidth, 21, TFT_WHITE);

  spriteBuffer.setTextFont (1);
  spriteBuffer.setTextSize (2);
  spriteBuffer.setTextDatum (TC_DATUM);
  spriteBuffer.setTextColor (TFT_BLACK, TFT_WHITE);
  spriteBuffer.drawString (_meteringModeDisplayName (), displayWidth / 2, overlayPosition + 3);
}

// ------------------------------------------------------------------- testOverload

bool testOverload (float min, float max) {
  bool modeCanDisableInputs = meteringModeSupportsDisablingInputs (meterMode);
  
  // Handle under-over voltages.
  // Always test source1.
  if ((rawData.source1 < min) || (rawData.source1 > max)) {
    return true;
  }
  
  // Only test source2 if more than 1 input is enabled.
  if ((meterData.inputEnabledCount < 2) && (modeCanDisableInputs)) {
    goto done;
  }
  if ((rawData.source2 < min) || (rawData.source2 > max)) {
    return true;
  }

  // Only test source3 if more than 2 inputs are enabled.
  if ((meterData.inputEnabledCount < 3) && (modeCanDisableInputs)) {
    goto done;
  }
  if ((rawData.source3 < min) || (rawData.source3 > max)) {
    return true;
  }

  // Only test source4 if more than 3 inputs are enabled.
  if ((meterData.inputEnabledCount < 4) && (modeCanDisableInputs)) {
    goto done;
  }
  if ((rawData.source4 < min) || (rawData.source4 > max)) {
    return true;
  }
  
  // Only test source5 if more than 4 inputs are enabled.
  if ((meterData.inputEnabledCount < 5) && (modeCanDisableInputs)) {
    goto done;
  }
  if ((rawData.source5 < min) || (rawData.source5 > max)) {
    return true;
  }

done:

  return false;
}

// ------------------------------------------------------------------- fps

void fps () {
  unsigned long now = millis ();
  unsigned long delta = now - lastTime;

  if (delta > 0) {
    float fps = 1000.0f / delta;
    Serial.println (fps);
  }

  lastTime = now;
}

// ------------------------------------------------------------------- handleInputEnabling

void handleInputEnabling () {
  // Store away a button press.
  meterData.buttonToggle = topButtonPressed ();
  if (meterData.buttonToggle && meteringModeSupportsDisablingInputs (meterMode)) {
    if (meterData.inputEnabledCount >= 5) {
      countingDown = true;
    } else if (meterData.inputEnabledCount <= 1) {
      countingDown = false;
    }
    if (countingDown) {
      meterData.inputEnabledCount = meterData.inputEnabledCount - 1;
    } else {
      meterData.inputEnabledCount = meterData.inputEnabledCount + 1;
    }
  }
}

// ------------------------------------------------------------------- loop

void loop () {
  // Read shared results safely
  int16_t adcSnapshot[8];
  portENTER_CRITICAL (&adcMux);
  memcpy (adcSnapshot, (void *) adcResults, sizeof (adcSnapshot));
  portEXIT_CRITICAL (&adcMux);

  // Assign and alpha.
  rawData.source1 = (alpha * (float) adcSnapshot[0]) + ((1.0 - alpha) * rawData.source1);
  rawData.source2 = (alpha * (float) adcSnapshot[1]) + ((1.0 - alpha) * rawData.source2);
  rawData.source3 = (alpha * (float) adcSnapshot[2]) + ((1.0 - alpha) * rawData.source3);
  rawData.source4 = (alpha * (float) adcSnapshot[3]) + ((1.0 - alpha) * rawData.source4);
  rawData.source5 = (alpha * (float) adcSnapshot[4]) + ((1.0 - alpha) * rawData.source5);
  rawData.rawUnitNegOne = (alpha * (float) adcSnapshot[5]) + ((1.0 - alpha) * rawData.rawUnitNegOne);
  rawData.rawUnitZero = (alpha * (float) adcSnapshot[6]) + ((1.0 - alpha) * rawData.rawUnitZero);
  rawData.rawUnitOne = (alpha * (float) adcSnapshot[7]) + ((1.0 - alpha) * rawData.rawUnitOne);
  
  // Handle overload (voltages getting too close to the rails).
  bool modeCanDisableInputs = meteringModeSupportsDisablingInputs (meterMode);
  bool overload = false;
  float minVoltage= rawData.rawUnitNegOne;
  float maxVoltage= rawData.rawUnitOne;

  meterData.source1Overloaded = false;
  meterData.source2Overloaded = false;
  meterData.source3Overloaded = false;
  meterData.source4Overloaded = false;
  meterData.source5Overloaded = false;

  // Always test input 1.
  if ((rawData.source1 < minVoltage) || (rawData.source1 > maxVoltage)) {
    meterData.source1Overloaded = true;
    overload = true;
  }

  // Only test source2 if more than 1 input is enabled.
  if ((meterData.inputEnabledCount < 2) && (modeCanDisableInputs)) {
    goto done;
  } else if ((rawData.source2 < minVoltage) || (rawData.source2 > maxVoltage)) {
    meterData.source2Overloaded = true;
    overload = true;
  }

  // Only test source3 if more than 2 inputs are enabled.
  if ((meterData.inputEnabledCount < 3) && (modeCanDisableInputs)) {
    goto done;
  } else if ((rawData.source3 < minVoltage) || (rawData.source3 > maxVoltage)) {
    meterData.source3Overloaded = true;
    overload = true;
  }
  
  // Only test source4 if more than 3 inputs are enabled.
  if ((meterData.inputEnabledCount < 4) && (modeCanDisableInputs)) {
    goto done;
  } else if ((rawData.source4 < minVoltage) || (rawData.source4 > maxVoltage)) {
    meterData.source4Overloaded = true;
    overload = true;
  }

  if ((meterData.inputEnabledCount < 5) && (modeCanDisableInputs)) {
    goto done;
  } else if ((rawData.source5 < minVoltage) || (rawData.source5 > maxVoltage)) {
    meterData.source5Overloaded = true;
    overload = true;
  }

done:

  digitalWrite (OVERLOAD_OUT_PIN, overload ? HIGH : LOW);
  
  // Rendering.
  // Display the data.
  handleMetering ();
  handleOverlay ();
  spriteBuffer.pushSprite (0, 0);

  // Test for the top button pressed.
  handleInputEnabling ();

  // Bump loop counter.
  loopCount = loopCount + 1;
  
  // fps();
}

// ------------------------------------------------------------------- setup

void setup() {
  // Kill Wi-Fi.
  WiFi.mode (WIFI_OFF);
  WiFi.disconnect (true);

  // Set up serial.
  Serial.begin (115200);  // Common baud rate for ESP32
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB
  }

  // Init TFT display.
  tft.init ();
  tft.setRotation (1);
  tft.fillScreen (TFT_BLACK);
  displayWidth = tft.width ();
  displayHeight = tft.height ();
  
  // Set up I2C.
  Wire.begin (SDA_PIN, SCL_PIN);
  Wire.setClock (100000);
  
  if (!adsSourcesGND.begin (ADS1X15_ADDRESS)) { // ADS1X15_ADDRESS == GND
    Serial.println ("Failed to initialize meters ADS.");
    while (1);
  }
  
  // Set gain so full-scale range.
  adsSourcesGND.setGain (GAIN_ONE);
  adsSourcesGND.setDataRate (RATE_ADS1015_3300SPS);  // ADS1015: RATE_ADS1015_3300SPS, ADS1115: RATE_ADS1115_860SPS
  
  if (!adsSources3V3.begin (ADS1X15_ADDRESS + 1)) {    // ADS1X15_ADDRESS + 1 == 3V3
    Serial.println ("Failed to initialize sources ADS.");
    while (1);
  }

  // Set gain so full-scale range.
  adsSources3V3.setGain (GAIN_ONE);
  adsSources3V3.setDataRate (RATE_ADS1015_3300SPS);  // ADS1015: RATE_ADS1015_3300SPS, ADS1115: RATE_ADS1115_860SPS
  
  // Pin adcTask to Core 0, loop() runs on Core 1
  xTaskCreatePinnedToCore (adcTask, "ADCTask", 2048, NULL, 1, NULL, 0);
  
  // Set up overload and experimental DAC output.
  pinMode (OVERLOAD_OUT_PIN, OUTPUT);
  // pinMode (EXP_DAC_PIN, OUTPUT);

  // Call utiltities to set up the button inputs (build into the T-Display).
  initUtilities ();

  // Create full-screen buffer (sprite).
  spriteBuffer.setColorDepth (16);
  spriteBuffer.setSwapBytes (false);
  spriteBuffer.createSprite (displayWidth, displayHeight);

  // Assign to meter data.
  meterData.buffer = &spriteBuffer;
  meterData.displayWidth = displayWidth;
  meterData.displayHeight = displayHeight;
  meterData.inputEnabledCount = 5;
  
  assignMeterMode (VERTICAL_METERINGMODE);

  Serial.println ("setup() COMPLETE!");
}
