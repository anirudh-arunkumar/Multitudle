#include "keyboard.h"
#include "doubly_linked_list.h"
#include "dictionary.h"
#include "dictionaryS.h"
#include "equation.h"


#include "globals.h"


#include <cstdlib>
#include <cstring>
#include <string>




int compareChar(void* input1, void* input2){
    if(*(char*)input1 == *(char*)input2){
        return 1;
    }
    else{
        return 0;
    }
}

DLinkedList* keyboard = create_dlinkedlist(compareChar);
LLNode* currNode = NULL;
char alphabetLetters[] = "abcdefghijklmnopqrstuvwxyz";
char numberCharacters[] = "1234567890+-=";
Equation rightEquation;
Word rightWord;
WordS rightWordS;
char wordGuess[] = {' ', ' ', ' ', ' ' ,' '};
int space = 0;
int totalGuesses = 0;


void init_keyboard(int seed)
{
    if (mode == 1) {
        char *alphabetHeap;
        alphabetHeap = (char *) malloc(26*sizeof(char));
        for(int i = 0; i < 26; i++){
            alphabetHeap[i] = alphabetLetters[i];
            insertTail(keyboard, &(alphabetHeap[i]));
        }
        currNode = getHead(keyboard);
        int size = sizeof(dictionary)/sizeof(dictionary[0]);
        int randomized = seed % size;
        rightWord = dictionary[randomized];
        pc.printf("Codeword: %s\n", rightWord.letters);
        uLCD.cls();
        uLCD.locate(0,0);
        drawCharacter(getData(currNode));
    } else if (mode == 2) {
        char *numberHeap;
        numberHeap = (char *) malloc(13*sizeof(char));
        for(int i = 0; i < 13; i++){
            numberHeap[i] = numberCharacters[i];
            insertTail(keyboard, &(numberHeap[i]));
        }
        currNode = getHead(keyboard);
        int size = sizeof(library)/sizeof(library[0]);
        int randomized = seed % size;
        rightEquation = library[randomized];
        uLCD.cls();
        uLCD.locate(0,0);
        pc.printf("Right Equation: %s\n", rightEquation.numbers);
        drawCharacter(getData(currNode));
    
    } else if (mode == 3) {
        char *alphabetHeap;
        alphabetHeap = (char *) malloc(26*sizeof(char));
        for(int i = 0; i < 26; i++){
            alphabetHeap[i] = alphabetLetters[i];
            insertTail(keyboard, &(alphabetHeap[i]));
        }
        currNode = getHead(keyboard);
        int size = sizeof(dictionaryS)/sizeof(dictionaryS[0]);
        int randomized = seed % size;
        rightWordS = dictionaryS[randomized];
        uLCD.cls();
        uLCD.locate(0,0);
        pc.printf("Right Word: %s\n", rightWordS.letters);
        drawCharacter(getData(currNode));
    
    }
}

void reset_guess() {
    if (mode == 3) {
        char wordGuess[] = {' ', ' ', ' ', ' ' ,' ', ' '};
    } else {
        char wordGuess[] = {' ', ' ', ' ', ' ' ,' '};
    }
    totalGuesses = 0;
    space = 0;
}

void moveleft()
{
    if (currNode->data == keyboard->head->data) {
        currNode = getTail(keyboard);
        drawCharacter(getData(currNode));
        char letter = *(char*)currNode->data;
        return;
    } else {
        currNode = getPrev(currNode);
    
        drawCharacter(getData(currNode));
    
        char letter = *(char*)currNode->data;
    }
}


void moveright()
{
    if (currNode->data == keyboard->tail->data) {
        currNode = getHead(keyboard);
        char letter = *(char*)currNode->data;
        drawCharacter(getData(currNode));
        
        return;
    } else {
        currNode = getNext(currNode);
        char letter = *(char*)currNode->data;
        drawCharacter(getData(currNode));
    
        
    }
}



void select_letterA()
{  
    if (mode == 3) {
        if(space < 6){ 
            wordGuess[space] = *(char*)currNode->data;
            space += 1;
            drawGuess(totalGuesses, wordGuess);
        }
    } else {
        if(space < 5){ 
            wordGuess[space] = *(char*)currNode->data;
            space += 1;
            drawGuess(totalGuesses, wordGuess);
        }
    }
}



void remove_letter()
{   
    wordGuess[space] = ' ';
    if(space > 0) {
        space--;
    }
    drawGuess(totalGuesses, wordGuess);
    
}




void check_word()
{
        if (mode == 1){
            char old[5] = {' ', ' ', ' ', ' ', ' '};
            char correct[5] = {' ', ' ', ' ', ' ', ' ' };
            const char *check = &rightWord.letters[0];
            int i;
            space = 0;
            const char *guess = &wordGuess[0];
            
        
            totalGuesses += 1;
            
            if(strcmp(guess, check) == 0){
                state = 1;
                mode = 5;
            } else if(totalGuesses > 5){
                state = 2;
                mode = 4;
            }
            for(i = 0; i < 5; i++){
                char letter = wordGuess[i];
                const char *target = &letter;
                pc.printf("%s\n", target);
                if(strchr(rightWord.unique, wordGuess[i]) != NULL){
                    if(strchr(correct, wordGuess[i]) == NULL){
                        strcat(correct, target);
                        pc.printf("good\n");
                    }
                } else {     
                   deleteNode(keyboard, findNode(keyboard, &wordGuess[i]));
                }
            }
            
        
            i = 0;
            for(i = 0; i < 5; i++){
                if(guess[i] == check[i]){
                    old[i] = guess[i];
                } else{
                    old[i] = ' ';
                }
            }
            
            
            i = 0;
            memset(wordGuess, ' ', strlen(wordGuess));
            pc.printf("%5s\n", old);
            pc.printf("%5s\n", correct);
            pc.printf("%i\n", totalGuesses);
            drawRightChar(correct);
            
            drawOldGuess(totalGuesses, old);
            
            for(i = 0; i < 5; i++){
                wordGuess[i] = ' ';
                old[i] = ' ';
            }
      } else if (mode == 2){
        char old[5] = {' ', ' ', ' ', ' ', ' '};
        char correct[5] = {' ', ' ', ' ', ' ', ' ' };
        const char *check = &rightEquation.numbers[0];
        int i;
        space = 0;
        const char *guess = &wordGuess[0];
        
    
        totalGuesses += 1;
        
        if(strcmp(guess, check) == 0){
            state = 1;
            mode = 5;
        } else if(totalGuesses > 5){
            state = 2;
            mode = 4;
        }
        for(i = 0; i < 5; i++){
            char letter = wordGuess[i];
            const char *target = &letter;
            pc.printf("%s\n", target);
            if(strchr(rightEquation.unique, wordGuess[i]) != NULL){
                if(strchr(correct, wordGuess[i]) == NULL){
                    strcat(correct, target);
                    pc.printf("good\n");
                }
            } else {     
               deleteNode(keyboard, findNode(keyboard, &wordGuess[i]));
            }
        }
        
    
        i = 0;
        for(i = 0; i < 5; i++){
            if(guess[i] == check[i]){
                old[i] = guess[i];
            } else{
                old[i] = ' ';
            }
        }
        
        
        i = 0;
        memset(wordGuess, ' ', strlen(wordGuess));
        pc.printf("%5s\n", old);
        pc.printf("%5s\n", correct);
        pc.printf("%i\n", totalGuesses);
        drawRightChar(correct);
        
        drawOldGuess(totalGuesses, old);
        
        for(i = 0; i < 5; i++){
            wordGuess[i] = ' ';
            old[i] = ' ';
        }
    } else if (mode == 3) {
        char old[6] = {' ', ' ', ' ', ' ', ' ', ' '};
        char correct[6] = {' ', ' ', ' ', ' ', ' ', ' '};
        const char *check = &rightWordS.letters[0];
        int i;
        space = 0;
        
        const char *guess = &wordGuess[0];
        
        totalGuesses += 1;
        
        if(strcmp(guess, check) == 0){
            state = 1;
            mode = 5;
        } else if(totalGuesses > 5){
            state = 2;
            mode = 4;
        }
        for(i = 0; i < 6; i++){
            char letter = wordGuess[i];
            const char *target = &letter;
            if(strchr(rightWordS.unique, wordGuess[i]) != NULL){
                if(strchr(correct, wordGuess[i]) == NULL){
                    strcat(correct, target);
                }
            } else {     
               deleteNode(keyboard, findNode(keyboard, &wordGuess[i]));
            }
        }
        
    
        i = 0;
        for(i = 0; i < 6; i++){
            if(guess[i] == check[i]){
                old[i] = guess[i];
            } else{
                old[i] = ' ';
            }
        }
        
    
        i = 0;
        memset(wordGuess, ' ', strlen(wordGuess));
        
        drawRightChar(correct);
        
        drawOldGuess(totalGuesses, old);
        
        for(i = 0; i < 6; i++){
            old[i] = ' ';
            wordGuess[i] = ' ';
            
        }
    }
}
