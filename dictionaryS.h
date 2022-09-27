struct WordS {
   char* letters;
   int counts[6];
   char unique[6];
} wordS;  


WordS dictionaryS[5] = {
    {"acting", {1,1,1,1,1,1}, {'a','c', 't', 'i', 'n', 'g'}},
    {"beauty", {1,1,1,1,1,1}, {'b','e', 'a', 'u', 't', 'y'}},
    {"client", {1,1,1,1,1,1}, {'c','l', 'i', 'e', 'n', 't'}},
    {"fiscal", {1,1,1,1,1,1}, {'f','i', 's', 'c', 'a', 'l'}},
    {"habits", {1,1,1,1,1,1}, {'h','a', 'b', 'i', 't', 's'}}
    };

extern WordS rightWordS;