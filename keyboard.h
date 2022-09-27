


#include "doubly_linked_list.h"

#include "hardware.h"
#include "graphics.h"

#include "globals.h"
/*
* Function to implement comparisons of data in the DLL.
* This function allows the comparisons of chars if you input
* chars into the doubly_linked_list. If you want you can make 
* additional functions to compare different types of data.

*/
extern DLinkedList* keyboard;
extern char wordGuess[5];
extern char wordGuessS[6];
extern char equationGuess[5];
extern LLNode* currNode;






int compareChar(char* input1, char* input2);

/*
* This function should intialize the keyboard and 
* select the goal word. To pick goal word, select a word 
* randomly from the dictionary you create in dictionary.h 
* (hint: the function rand() can be used to generate a random
* number >=0).
* To create the keyboard, initialize a DLL with all the nodes
* being the letters of the alphabet. Display the visible part
* of the keyboard at the start of the game (e.g., for baseline: 
* display the first letter of the alphabet).
*/
void init_keyboard(int seed);

/*
* Scroll left on the keyboard.
*/
void moveleft();

/*
* Scroll right on the keyboard.
*/
void moveright();

/*
* Pick the current letter in the keyboard to use 
* in your guess.
*/
void select_letterA();
void select_letterB();
void select_letterC();

/*
* Removes letters from your guess that you no
* longer want in your guess (Removes right to left).
*/
void remove_letter();

void reset_guess();

/*
* This function compares your guess to the secret word. If
* you guess the correct letter in the correct spot, keep the 
* letter displayed. If you guess the correct letter in the
* incorrect spot, display that letter on the top of the screen.
* Delete letters that are not in the secret word from the keyboard
* DLL. Also should handle game winning and game losing.
*
*/
void check_word();
