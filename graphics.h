#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "globals.h"
 

// The bottom of the screen => y=127
// A macro to reverse the coordinate for convenience (optional).
#define REVERSE_Y(x) (SIZE_Y-(x))

/**
 * Draws a sprite
 */
void draw_stickman();
void draw_lower_status();
void drawHeart();
void drawStart();
void drawModeSelect();
void drawWin();
void drawLoss();
void drawHi();
void drawTime();
void drawSmile();
void drawFrown();
void drawSunOne();
void drawSunTwo();
void drawSunThree();
void drawSunFour();
void drawSunFive();
void drawCharacter(void* data);
void drawOldGuess(int guessCount, char* oldGuess);

void drawGuess(int guessCount, char* guessString);



void drawRightChar(char* correctLetters);




#endif // GRAPHICS_H
