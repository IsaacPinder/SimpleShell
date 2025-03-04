#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <unistd.h>    // fork, execl
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h>  // wait
#include <errno.h> 

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
  if (str_len(str1) != str_len(str2)){
    return -1;
  }
  
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

int str_exec_num(char* input1){
// make sure not to change index(make copy)
// take an input check to make sure first char is '!'

// if its '!' followed by a num directly then
//  return the number aslong as 1 or 2 digits (<=19)

// should check that number is not followed by any more chars
// if failure then return -1
return 0;
}
int str_exec_num_minus(char* input1,int index){
// make sure not to change index(make copy)
// take an input check to make sure first char is '!'
// check second char is '-'
//    note need to return (index - num) taking account for circular array
//    return the number which follows '-' aslong as its 1 or 2 digit (<=19)


// should check that number is not followed by any more chars
// if failure then return -1
return 0;
}



int add_history(char *histarr[20], int commandIndex, char* command){
  // if array has something in then free malloc for that location
  if(histarr[commandIndex] != NULL){
    free(histarr[commandIndex]);
  }
  // malloc array location for size of gthe input
  histarr[commandIndex] = malloc(sizeof(str_len(command)));
  // copy command into array
   strcpy(histarr[commandIndex],command);
   // return new index
   return commandIndex = (commandIndex + 1) % 20;
}

void tokenise(char* tokensarr[],char* line){
  // tolkenises the line string by provided delimeters e.g (" \t|><&;")
  char *token = strtok(line, " \t|><&;\n");

  // token index to fill array
  int toki = 0;
  // loops through line string breaking it into smaller strings until end of line and stores in array

  

  while (token != NULL)
  {
    // store token in array
    tokensarr[toki] = token;

    // this print shows tolkens
    // printf("%s", token);

    // go to next token
    token = strtok(NULL, " \t|><&;\n");

    // increment array
    toki += 1;
  }
  // make final array item NULLs
  tokensarr[toki] = NULL;
}

