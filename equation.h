struct Equation {
   char* numbers; //letters
   int counts[5];
   char unique[5];
} equation;  


Equation library[8] = {
    {"1+4=5", {1,1,1,1,1}, {'1','+', '4', '=', '5'}},
    {"2+4=6", {1,1,1,1,1}, {'2','+', '4', '=', '6'}},
    {"3+1=4", {1,1,1,1,1}, {'3', '+', '1', '=', '4'}},
    {"6+2=8", {1,1,1,1,1}, {'6', '+', '2', '=', '8'}},
    {"7-4=3", {1,1,1,1,1}, {'7', '-', '4', '=', '3'}},
    {"8-6=2", {1,1,1,1,1}, {'8', '-', '6', '=', '2'}},
    {"9-8=1", {1,1,1,1,1}, {'9', '-', '8', '=', '1'}},
    {"4+3=7", {1,1,1,1,1}, {'4', '+', '3', '=', '7'}},
    };

extern Equation rightEquation;