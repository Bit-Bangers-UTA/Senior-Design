/*
 * Bit Bangers OPT3101 Whisker Library
 * github.com/Bit-Bangers-UTA/Senior-Design
 *
 * Authors:
 * Rolando Rosales
*/

#ifndef OPT3101_WHISKER_H_
#define OPT3101_WHISKER_H_

// Libraries ------------------------------------------------------------------

#include <Arduino.h>
#include <OPT3101.h>
#include <Wire.h>
#include "OPT3101_whisker.h"

// Defines --------------------------------------------------------------------

// Variables & Constants ------------------------------------------------------

// Structures & Classes -------------------------------------------------------

// Functions ------------------------------------------------------------------

void initOPT3101(void);
int16_t getWhiskerDistanceCm(void);

#endif