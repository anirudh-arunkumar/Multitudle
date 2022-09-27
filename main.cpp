//=================================================================
// The main program file.
//
// Copyright 2022 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

#include <stdlib.h>

#include "globals.h"
#include "hardware.h"
#include "graphics.h"
#include "keyboard.h"
#include "mbed_wait_api.h"


//some LED's to use for debugging.
DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

int set_random_seed();
int state = 0;
int mode = 0;
/*
* This function handles the main logic of the game. You should
* initialize the hardware in this function, make calls to 
* functions that update the keyboard and your guess of 
* the word. 
*/
int main()
{
    
    GameInputs inputs; 
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    pc.printf("Program Starting\n");
    //led1 hardware initialization
    
    myled1=1;  
    Timer t;
    int dt; // delta time
    
/* Put code here to initialize the game state:
   1) you will need to use a psuedo random number generator such as
     rand() to randomly select a goal word from the dictionary.
     The random function needs to be seeded before you call rand(),
     otherwise, rand() will always generate the same series of numbers.
     Call the function set_random_seed -- you need to complete its
     definition below.*/
    drawStart();
    draw_stickman();
    drawHeart();
    drawHi();
    drawTime();
    while(1){
        inputs = read_inputs();
        if (inputs.b1 == 1 || inputs.b2 == 1 || inputs.b3 == 1){
            break;
        }
    }
    wait_ms(1000);
    drawModeSelect();
    drawSmile();
    drawFrown();    
    while(1){
        inputs = read_inputs();
        if (inputs.b1 == 1) {
           mode = 3;
           break;
       } else if(inputs.b2 == 1) {
           mode = 2;
           break;
       } else if(inputs.b3 == 1) {
           mode = 1;
           break;
       }
    }
    
    int c;
    for (c = 0; c < 5; c++) {
        drawSunOne();
        wait_ms(250);
        drawSunTwo();
        wait_ms(250);
        drawSunThree();
        wait_ms(250);
        drawSunFour();
        wait_ms(250);
        }
        drawSunFive();
        
        wait_ms(120);
    
    while (state == 0) {
    init_keyboard(set_random_seed());
    
    wait_ms(500);
/*
   2) call init_keyboard() and show start screen
   
   3) initialize any other game state that you add.
*/
    
    
/* Insert code into this game loop to complete its implementation:
*/      if (mode == 3){
            reset_guess();
            wait_ms(220);
        }
        int count = 0;
        while(state == 0){
            t.start(); //start a timer
            
            draw_lower_status(); //draw the lower status bar
            
            inputs = read_inputs(); //read the inputs of the game
                
                
                
                if(inputs.b1 == 1){
                    moveright();
                    wait_ms(220);
                }
                
        
                
                if(inputs.b3 == 1){
                    select_letterA();
                    count = count + 1;
                    wait_ms(500);
                    
                } 
                
                if(inputs.b2 == 1){
                    moveleft();
                    wait_ms(220);
                    
                } 
                
                if (mode == 3) {
                    if (count == 6) {
                        check_word();
                        count = 0;
                        wait_ms(500);
                    }
                } else {
                    if (count == 5) {
                        check_word();
                        count = 0;
                        wait_ms(500);
                    }
                }
                
            if (inputs.b1 == 1 && inputs.b2 == 1 && inputs.b3 == 1) {
                reset_guess();
                break;
                
            }
    
    
    
    
    
            
            t.stop();
            //time = time + t.read();
            
            dt = t.read_ms();
            if (dt < 200) wait_ms(200 - dt);
    
    
            }
        
        
        
        
        
        wait_ms(250);
        
        
        if(state == 2 && mode == 4){
            drawLoss();
            uLCD.locate(0, 14);
            uLCD.printf("%f seconds", t.read());
        } else if(state == 1 && mode == 5){
            drawWin();
            uLCD.locate(0, 14);
            uLCD.printf("%f seconds", t.read());
        }
        
}
}


/* This should be called in the main function above.
*
* This function sets the random seed for the game using srand(seed).
* One incomplete way to do this is given below: start a Timer and sample it
* to get a seed. The problem is that the same exact time will be read
* every time you reset the mbed.  You need to add code to introduce
* variability into when the timer is stopped.  An easy way to do this
* is to get the user to press a push button to stop the timer at a random
* time.  You can print a prompt to the screen to ask the user to push
* any button to start, then run an infinite while loop that waits for a push 
* button press to break. The Timer is then stopped and read and elapsed time
* used as the seed. (This requires using read_inputs() into a GameInputs variable.)
*/
int set_random_seed() {
    Timer t;
    t.start(); // start the timer
    GameInputs inputs = read_inputs();
    

    while(inputs.b1 == 0 && inputs.b2 == 0 && inputs.b3 == 0){
        inputs = read_inputs();
    }
    
    
    
    
    
    
    t.stop();  //  end the timer
    int Rseed = t.read_ms(); //read the number of milliseconds elapsed between the start and stop
    srand(Rseed); // use elapsed time as the seed
    return Rseed;
}
// ===User implementations end===
