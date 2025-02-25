#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <unistd.h>    // fork, execl
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h>  // wait
#include <errno.h>     // errno

// ** Commands for compiling and running **
// gcc simpleshell.c helpers.c localfunctions.c -pedantic -Wall -o simpleshell
/// ./simpleshell

// Prompt function prints £: and takes user input
// upon "exit" or ^D being entered shell will quit

void prompt()
{
  // int exit to for use in main loop to determine if we should continue running
  int exitloop = 0;
  // pointer to strings for use with str_cmp to compare input to
  char *exitstr = "exit";
  char *getpathstr = "getpath";
  char *setpathstr = "setpath";
  char *cdstr = "cd";
  char *histstr = "history";
  char *execprevstr = "!!";
  



  // array to hold 20 history items
  char* history[20];
  // index of current command
  int commandIndex = 0;

  // save Home directory
  char *originDir = getenv("HOME");
  // print home and success status of changing to home
  printf("\nHomeDir : %s \n\n", originDir);
  printf(" chdir to home success? 0 good: %d \n\n", chdir(originDir));

  // malloc space for original path then copy the path into that space
  char *origPath = malloc(sizeof(str_len(getenv("PATH"))));
  strcpy(origPath, getenv("PATH"));
  printf("Original_Path : %s \n\n", origPath);

  // comented to avoid no use error
  // char* curPath =getenv("PATH");

  // main loop continue while exit != 1
  while (exitloop == 0)
  {
    printf("£: ");
    // buffer to take in input
    char input[512];
    // array to hold tokens
    char *tokensarr[50];
    // take in user input
    char *line = fgets(input, sizeof(input), stdin);

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
    // make final array item NULL
    tokensarr[toki] = NULL;

    // trim whitespace from input
    // char *trminput = str_trim(input);

    // EXIT: if ^D is pressed(therefor line is NULL) OR User input is NOT empty AND first token is "exit" AND second is empty ∂then done looping

    if (line == NULL || ((tokensarr[0] != NULL) && str_cmp(tokensarr[0], exitstr) == 0 && tokensarr[1] == NULL))
    {
      exitloop = 1;
    }
    // GETPATH: checking input isn't empty AND first token is "getpath"
    else if ((tokensarr[0] != NULL) && str_cmp(tokensarr[0], getpathstr) == 0)
    {
      // if too many arguments (2nd token not empty) print error
      if (tokensarr[1] != NULL)
      {
        printf("ERROR there is too many arguments given please amend your command\n");
      }
      // else print path
      else
      {
        printf("current path: \n %s\n", get_path());
      }
    }
    // SETPATH: checking input isnt empty AND first token is "setpath"
    else if ((tokensarr[0] != NULL) && str_cmp(tokensarr[0], setpathstr) == 0)
    {
      // if no path argument (2nd token empty) print error
      if (tokensarr[1] == NULL)
      {
        // missing path
        printf("ERROR there is no path given, Please enter a path\n");
      }
      // else if 3rd token is not empty (too many arguments)
      else if (tokensarr[2] != NULL)
      {
        // too many arguments
        printf("ERROR there is too many arguments given, please amend your command\n");
      }
      // else set path
      else
      {
        set_path(tokensarr[1]);
        // print path
        printf("setting path: \n %s\n", tokensarr[1]);
      }
    }
    // CD: checking input isnt empty AND first token is "cd"
    else if ((tokensarr[0] != NULL) && str_cmp(tokensarr[0], cdstr) == 0)
    {
      // if no arguments (2nd token empty) change to home directory
      if (tokensarr[1] == NULL)
      {
        // set directory to home directory
        chdir(originDir);
      }
       // else if 3rd token is not empty (too many arguments)
       else if (tokensarr[2] != NULL)
       {
         // too many arguments
         printf("ERROR there is too many arguments given, please amend your command\n");
       }
      // else change directory to paramter
      else
      {
        change_directory(tokensarr[1]);
      }
    }
    //HISTORY checking input isnt empty AND first token is "history"
    else if((tokensarr[0] != NULL) && str_cmp(tokensarr[0], histstr) == 0){
       if (tokensarr[2] != NULL)
       {
         // too many arguments
         printf("ERROR there is too many arguments given, please amend your command\n");
       }
       else{
        print_history(history,commandIndex);
       }
    }
    //EXECUTE-PREVIOUS checking input isnt empty AND first token is "!!"
    else if((tokensarr[0] != NULL) && str_cmp(tokensarr[0], execprevstr) == 0){
      if (tokensarr[2] != NULL)
      {
        // too many arguments
        printf("ERROR there is too many arguments given, please amend your command\n");
      }
      else if(history[0] == NULL){
         // no commands in history
         printf("ERROR there is no commands in history, please execute some commands\n");
      }
      else{
       execute_command(history,commandIndex);
      }
    }
    //EXECUTE-COMMAND-NUM checking input isnt empty AND input is "!n" use n to execute comand
    else if((tokensarr[0] != NULL) && str_exec_num(tokensarr[0],commandIndex) >= 0){
      // returns the number to execute
      int num_to_exec = str_exec_num(tokensarr[0],commandIndex);
      execute_command(history,num_to_exec);
    }
    //EXECUTE-COMMAND-MINUS checking input isnt empty AND input is '!-n' use a subtracted n to execute comand
    else if((tokensarr[0] != NULL) && str_exec_num_minus(tokensarr[0],commandIndex) >= 0){
      // returns the number to execute
      int num_to_exec = str_exec_num_minus(tokensarr[0],commandIndex);
      execute_command(history,num_to_exec);
    }


    // else ask operating system for command
    else
    {
      // fork
      pid_t p = fork();

      // if < 0 then error happened
      if (p < 0)
      {
        printf("error forking errno = %d\n", errno);
        exit(1);
      }
      // child process
      else if (0 == p)
      {
        // execvp takes tokens
        execvp(tokensarr[0], tokensarr);
        // print error
        perror(tokensarr[0]);
        // exit
        exit(1);
      }
      // parent process (wait for child)
      else
      {
        wait(NULL);
      }
    }
  }

  // set currentpath to orignalpath then free malloch
  set_path(origPath);
  printf("path set on exit: \n %s\n", get_path());
  free(origPath);
  origPath = NULL;

  // exit confirmation message
  printf("yay u left\n");
}

int main(void)
{
  prompt();
  return 0;
}
