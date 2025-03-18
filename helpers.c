#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <unistd.h>    // fork, execl
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h>  // wait
#include <errno.h>
#include <ctype.h>

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
  if (str_len(str1) != str_len(str2))
  {
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
int whitespace(char c)
{
  return (c == ' ' || c == '\t');
}

// Coded by Isaac during Semester 1 excercises
// for use in str_trim, iterates through string, removes whitespace charcters as defined in whitespace function
char *str_trim(char *str)
{
  int startpos = 0;
  int endpos = str_len(str) - 1;

  for (int i = 0; i < str_len(str); i++)
  {

    if (!whitespace(*(str + i)))
    {
      startpos = i;
      break;
    }
  }

  for (int j = endpos; j >= 0; j--)
  {
    if (!whitespace(*(str + j)))
    {
      endpos = j;
      break;
    }
  }

  if (startpos > endpos)
  {
    *str = '\0';
    return str;
  }

  *(str + endpos + 1) = '\0';
  char *newpoint = str + startpos;

  return newpoint;
}

int str_exec_num(char* input, int index, char *histarr[]) {
  if (input[0] == '!') {
          // handle positive numbers
          if (isdigit(input[1]) == 0) {
              return -1;
          }
      
          if (input[2] != '\0' && isdigit(input[2]) == 0) {
              printf("3rd input is not a number and you've not got it empty  \n");
              return -1;
          }
  
          if (input[2] != '\0' && input[3] != '\0') {
              return -1;
          }


          int n = atoi(input + 1);
          // so checks range of number without !
          if (n >= 0 && n <= 19) {
          if (histarr[19] != NULL && index < 19){
          return (n + index)%20;
          } 
          else { return n ;}
          }
  }

      return -1;

  }



int str_exec_num_minus(char *input1, int index){
  if (input1[0] == '!'){
    if (input1[1] == '-') {
          if (isdigit(input1[2]) == 0) {
              return -1;
          }
      
          if (input1[3] != '\0' && isdigit(input1[3]) == 0) {
              return -1;
          }
  
          if (input1[3] != '\0' && input1[4] != '\0') {
              return -1;
          }
          // for when head and tail isn;t moved then 
          // arayysize - 1 then take tail go from there and check if looped
  
          int n = atoi(input1 + 1);
          // so checks range of number without !
          // n is a negative number at this point
          if (n <= -1 && n >= -20) {
            
            return (index + (n+20)) % 20;            
          }
    }
  
}
return -1;
}

int add_history(char *histarr[20], int commandIndex, char *command)
{
  // if array has something in then free malloc for that location
  if (histarr[commandIndex] != NULL)
  {
    free(histarr[commandIndex]);
  }
  // malloc array location for size of gthe input
  histarr[commandIndex] = malloc(sizeof(str_len(command)));
  // copy command into array
  strcpy(histarr[commandIndex], command);
  // return new index
  return commandIndex = (commandIndex + 1) % 20;
}

void tokenise(char *tokensarr[], char *line)
{
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

int getFromFile(char *histarr[20]) {

  // opens file in read mode
  FILE *fptr = fopen("History.txt", "r");
  // if fails print error return 0 as defualt command index
  if (fptr == NULL) {
      printf("No History file found\n");
      return 0;
  }

  printf("History.txt opened\n");

  int commandIndex = 0;

  // reads in command index from file (first item)
  fscanf(fptr, "%d\n", &commandIndex);
  printf("Command index read: %d\n", commandIndex);

  // Temporary buffer for read in line
  char buffer[512]; 

  int i = 0;

  while (i < 20 && fgets(buffer, sizeof(buffer), fptr)) {

    //convert the read in line to pointer for using in our addhistory function
      char* bufpoint = buffer;
      add_history(histarr,i,bufpoint);
     
      i++;
  }

  //close file
  fclose(fptr);

  return commandIndex;
}
  


//function probably shouldnt be void just wrote for quickness
  void sendToFile(int commandIndex, char *histarr[20])
  {
    int i = 0;

    FILE *fptr;
     //opens file
     fptr = fopen("History.txt", "w");

     if (fptr == NULL) {
      printf("\nError opening file\n"); 
      return;
  }

      //prints command index in file first
      //  fprintf(fptr, "%d", commandIndex);

        //sends what is in histarr to file 
         while (i < 20 && histarr[i] != NULL){

          //print array items to double check has stuff
          printf("histarr[%d]: %s\n", i, histarr[i]);

          fprintf(fptr,"%s\n",histarr[i]);

          i = i + 1;
        }

    //closes file
    fclose(fptr);
     }
  


