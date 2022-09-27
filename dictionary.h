/*
* This file contains the dictionary array which is 
* an array of Word structs that hold words that could be
* potential secret words. You may add or remove from the Word
* struct as you see fit for your own implementation.
* You should add more potential secret words to the dictionary.
*/

struct Word {
   char* letters;
   int counts[5];
   char unique[5];
} word;  


Word dictionary[13] = {
    {"speed", {1,1,2,1,0}, {'s','p', 'e', 'd', '!'}},
    {"steel", {1,1,2,1,0}, {'s','t', 'e', 'l', '!'}},
    {"crane", {1,1,1,1,1}, {'c', 'r', 'a', 'n', 'e'}},
    {"virus", {1,1,1,1,1}, {'v', 'i', 'r', 'u', 's'}},
    {"tweak", {1,1,1,1,1}, {'t', 'w', 'e', 'a', 'k'}},
    {"brief", {1,1,1,1,1}, {'b', 'r', 'i', 'e', 'f'}},
    {"decoy", {1,1,1,1,1}, {'d', 'e', 'c', 'o', 'y'}},
    {"liter", {1,1,1,1,1}, {'l', 'i', 't', 'e', 'r'}},
    {"flame", {1,1,1,1,1}, {'f', 'l', 'a', 'm', 'e'}},
    {"scald", {1,1,1,1,1}, {'s', 'c', 'a', 'l', 'd'}},
    {"guess", {1,1,1,2,0}, {'g', 'u', 'e', 's', '!'}},
    {"stock", {1,1,1,1,1}, {'s', 't', 'o', 'c', 'k'}},
    {"curry", {1,1,2,1,0}, {'c','u', 'r', 'y', '!'}}
    };

extern Word rightWord;





