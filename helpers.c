#include <stdio.h>
#include <string.h>
#include "header.h"

// Coded by Isaac during Semester 1 excercises
// Function takes  pointer to string and returns int length
int str_len(char *str)
{
  int valid = 0;
  int count = 0;
  while (!valid)
  {
    if (*str == '\0')
    {
      valid = 1;
    }
    else
    {
      count++;
      str++;
    }
  }
  return count;
}

// Coded by Isaac during Semester 1 excercises
// function takes 2 pointers to 2 strings and returns 0 if they are equal
int str_cmp(char *str1, char *str2)
{
  for (int i = 0; i < str_len(str1); i++)
  {

    if (*(str1 + i) > *(str2 + i))
    {
      return 1;
    }

    if (*(str1 + i) < *(str2 + i))
    {
      return -1;
    }
  }

  return 0;
}
// Coded by Isaac during Semester 1 excercises 
// for use in str_trim
int whitespace(char c){
 return ( c ==' ' || c =='\t');
}


// Coded by Isaac during Semester 1 excercises 
// for use in str_trim, iterates through string, removes whitespace charcters as defined in whitespace function
char *str_trim(char *str){
int startpos = 0;
int endpos = str_len(str) - 1;

for(int i = 0; i < str_len(str);i++){

if (!whitespace(*(str + i))){
startpos = i;
break;
}
}


for(int j = endpos; j >= 0; j--){
if (!whitespace(*(str + j))) {
            endpos = j;
            break; 
}
}



if (startpos > endpos) {
        *str = '\0'; 
        return str;
    }


*(str + endpos + 1) = '\0';
char *newpoint = str + startpos;

return newpoint;
}
