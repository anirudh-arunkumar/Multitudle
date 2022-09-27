//=================================================================
// The header file for general settings for the project
//
// Copyright 2022 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

#ifndef GLOBAL_H
#define GLOBAL_H
#include "mbed.h"
#include "wave_player.h"
#include "uLCD_4DGL.h"
#include "MMA8452.h"
#include <math.h>

/*
This file holds all the global variables that are accessable from all the 
different files in this project. You see below most of it looks like hardware.
That's because it is hardware! The uLCD object is how you interact with the LCD
screen. The sd variable is how you interact with the sd card and so on for all
the other variables.
*/
extern uLCD_4DGL uLCD;      // LCD Screen
extern Serial pc;           // USB Console output
extern MMA8452 acc;         // Accelerometer
extern DigitalIn button1;   // Pushbuttons
extern DigitalIn button2;
extern DigitalIn button3;
extern AnalogOut DACout;    // Speaker
extern PwmOut speaker;
extern wave_player waver;




extern int state;
extern int mode;

// === [global settings] ===
// === [define the macro of error handle function] ===
// when the condition (c) is not true, assert the program and show error code
#define ASSERT_P(c,e) do { \
    if(!(c)){ \
        uLCD.printf("\nERROR:%d\n",e); \
        pc.printf("\nERROR:%d\n",e); \
        while(1); \
    } \
} while (0)

// === [error code] ===
#define ERROR_NONE 0 // All good in the hood

#define ERROR_DLL_INSERT_HEAD             -14 // inserting into doubly linked list at head failed
#define ERROR_DLL_DELETE                  -15 // deleting node from doubly linked list failed
// add other error codes ...

#endif //GLOBAL_H
