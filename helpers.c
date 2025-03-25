#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <unistd.h>    // fork, execl
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h>  // wait
#include <errno.h>
#include <ctype.h>


int str_exec_num(char *input, int index, char *histarr[])
{
  if (input[0] == '!')
  {
    // handle positive numbers
    if (isdigit(input[1]) == 0)
    {
      return -1;
    }

    if (input[2] != '\0' && isdigit(input[2]) == 0)
    {
      printf("3rd input is not a number and you've not got it empty  \n");
      return -1;
    }

    if (input[2] != '\0' && input[3] != '\0')
    {
      return -1;
    }

    int n = atoi(input + 1);
    // so checks range of number without !
    if (n >= 0 && n <= 19)
    {
      if (histarr[19] != NULL && index < 19)
      {
        return (n + index) % 20;
      }
      else
      {
        return n;
      }
    }
  }

  return -1;
}

int str_exec_num_minus(char *input1, int index)
{
  if (input1[0] == '!')
  {
    if (input1[1] == '-')
    {
      if (isdigit(input1[2]) == 0)
      {
        return -1;
      }

      if (input1[3] != '\0' && isdigit(input1[3]) == 0)
      {
        return -1;
      }

      if (input1[3] != '\0' && input1[4] != '\0')
      {
        return -1;
      }
      // for when head and tail isn;t moved then
      // arayysize - 1 then take tail go from there and check if looped

      int n = atoi(input1 + 1);
      // so checks range of number without !
      // n is a negative number at this point
      if (n <= -1 && n >= -20)
      {

        return (index + (n + 20)) % 20;
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
  // malloc array location for size of the input
  histarr[commandIndex] = malloc(strlen(command) + 1);
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

int getFromFile(char *histarr[20],char *origDir)
{

  // make a tempory copy of home dir location (to not overwrite original)
  char histfilelocation[512];

  // copy home dir to temp variable
  strcpy(histfilelocation, origDir);
  // concatenate hist to the rest of the line(from the end of first word)
  strcat(histfilelocation, "/.hist_list");

  // opens file in read mode
  FILE *fptr = fopen(histfilelocation, "r");
  // if fails print error and create file return 0 as defualt command index
  if (fptr == NULL)
  {
    printf("History not found Creating History file \n");
    FILE *fptr = fopen(histfilelocation, "w");
    fclose(fptr);
    return 0;
  }

  printf("History.txt opened\n");

  // Temporary buffer for read in line
  char buffer[512];

  int i = 0;

  // while index < 20 and line is succesfully read from file
  while (i < 20 && fgets(buffer, sizeof(buffer), fptr))
  {

    // convert the read in line to pointer for using in our addhistory function
    char *bufpoint = buffer;
    // add read in line to history
    add_history(histarr, i, bufpoint);

    i++;
  }

  // close file
  fclose(fptr);
  // return command index
  return i % 20;
}

void sendToFile(int commandindex, char *histarr[20], char *origdir)
{

  // make a tempory copy of home dir location (to not overwrite original)
  char histfilelocation[512];

  // copy home dir to temp variable
  strcpy(histfilelocation, origdir);
  // concatenate hist to the rest of the line(from the end of first word)
  strcat(histfilelocation, "/.hist_list");

  int i = 0;

  FILE *fptr;
  // opens file
  fptr = fopen(histfilelocation, "w");


  if (fptr == NULL)
  {
    printf("\nError opening file\n");
    return;
  }
  
  // sends what is in histarr to file
  // if last item is empty (not looped over circle) then write for length of history
  if (histarr[19] == NULL)
  {

    while (histarr[i] != NULL)
    {

      // print array items to double check has stuff
      //printf("histarr[%d]: %s\n", i, histarr[i]);

       fprintf(fptr, "%s", histarr[i]);

       // free malloc
       free(histarr[i]);
       // set to null
       histarr[i] = NULL;

      i++;
    }
  }

  // if looped over circle array
  else
  {
    // write from tail to end
    for (int tail = commandindex; tail < 19; tail++)
    {
      // write to file
      fprintf(fptr, "%s", histarr[tail]);
       // free malloc
       free(histarr[tail]);
       // set to null
       histarr[tail] = NULL;
    }
    // write from start to tail
    for (int start = 0; start < commandindex; start++)
    {
      // write to file
      fprintf(fptr, "%s", histarr[start]);
       // free malloc
       free(histarr[start]);
       // set to null
       histarr[start] = NULL;
    }
  }

  // closes file
  fclose(fptr);
}

void addToAlias(char *alias[10][2], char *name, char *command)
{

  // loop entire list to see if alias already exists
  for (int i = 0; i < 10; i++)
  {
    // check if null so strcmp dosnt break
    if (alias[i][0] != NULL)
    {
      if (strcmp(alias[i][0], name) == 0)
      {
        printf("Error alias already exists\n");
        return;
      }
    }
  }

  // loop through list find first empty space
  for (int i = 0; i < 10; i++)
  {
    // if empty space
    if (alias[i][0] == NULL)
    {

      // malloc array location for size of the input

      alias[i][0] = malloc(strlen(name) + 1);
      alias[i][1] = malloc(strlen(command) + 1);

      // copy input to array
      strcpy(alias[i][0], name);
      strcpy(alias[i][1], command);

      // return once found
      return;
    }
  }

  // if reach end of loop no empty spaces
  printf("Alias is at max capacity\n");
}

void removeAlias(char *alias[10][2], char *name)
{
  // loop through entire list
  for (int i = 0; i < 10; i++)
  {

    // check if null so strcmp dosnt break
    if (alias[i][0] != NULL)
    {
      // if alias found
      if (strcmp(alias[i][0], name) == 0)
      {

        // free malloc
        free(alias[i][0]);
        free(alias[i][1]);
        // set to null
        alias[i][0] = NULL;
        alias[i][0] = NULL;

        // return once found
        return;
      }
    }
  }
  // if reach end of loop alias not found
  printf("Alias not found\n");
}

void printAlias(char *alias[10][2])
{
  // count for number of things in alias(since we could have gaps in array)
  int count = 0;
  for (int i = 0; i < 10; i++)
  {
    // check value isnt null (a gap)
    if (alias[i][0] != NULL)
    {
      printf("%d: %s, %s\n", count, alias[i][0], alias[i][1]);
      count++;
    }
  }

  // if no items in alias
  if (count == 0)
  {
    printf("Alias is empty\n");
  }
}

void saveAliasToFile(char *alias[10][2], char *origdir)
{

// make a tempory copy of home dir location (to not overwrite original)
char aliasfilelocation[512];

// copy home dir to temp variable
strcpy(aliasfilelocation, origdir);
// concatenate hist to the rest of the line(from the end of first word)
strcat(aliasfilelocation, "/.aliases");


  FILE *fptr;
  // opens file
  fptr = fopen(aliasfilelocation, "w");
  // w for write
  if (fptr == NULL)
  {
    printf("No Alias file found\n");
  }

  for (int i = 0; i < 10; i++)
  {

    // write non gaps to file
    if (alias[i][0] != NULL && alias[i][1] != NULL)
    {
      fprintf(fptr, "%s %s\n", alias[i][0], alias[i][1]);
      // free malloc
      free(alias[i][0]);
      free(alias[i][1]);
      // set to null
      alias[i][0] = NULL;
      alias[i][0] = NULL;
    }
  }

  fclose(fptr);
}

void loadAliasFile(char *alias[10][2], char *origdir)
{

  // make a tempory copy of home dir location (to not overwrite original)
char aliasfilelocation[512];

// copy home dir to temp variable
strcpy(aliasfilelocation, origdir);
// concatenate hist to the rest of the line(from the end of first word)
strcat(aliasfilelocation, "/.aliases");

  FILE *fptr;
  // opens file
  fptr = fopen(aliasfilelocation, "r");

  if (fptr == NULL)
  {
  // if fails print error and create file 
    printf("Alias not found Creating Alias file \n");
    FILE *fptr = fopen(aliasfilelocation, "w");
    fclose(fptr);
    return ;
  }
  
  // Temporary buffer for read in line
  char buffer[512];
  int i = 0;

  // while index < 20 and line is succesfully read from file
  while (i < 10 && fgets(buffer, sizeof(buffer), fptr))
  {

    // tokenise from read in line (buffer)

    // take name/alias as first thing delimted by a space 
    char *name = strtok(buffer, " ");
    // take command as second thing delimited by newline
    char *command = strtok(NULL, "\n");

      addToAlias(alias, name, command);
      i++;
    
  }
  fclose(fptr);
}
