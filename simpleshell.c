#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <unistd.h>    // fork, execl
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h>  // wait
#include <errno.h>     // errno

// ** Commands for compiling and running **
// gcc simpleshell.c helpers.c -pedantic -Wall -o simpleshell
/// ./simpleshell

// Coded together as a full group
// Prompt function prints £: and takes user input
// upon "exit" or ^D being entered shell will quit

void prompt()
{
  // int exit to for use in main loop to determine if we should continue running
  int exitloop = 0;
  // pointer to string for use with str_cmp to compare input to "exit"
  char *exitstr = "exit\n";

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
    // if ^D is pressed(therefor line is NULL) OR User input is NOT empty AND first token is "exit" AND second is empty ∂then done looping

    if (line == NULL || ((tokensarr[0] != NULL) && str_cmp(tokensarr[0], exitstr) == 0 && tokensarr[1] == NULL))
    {
      exitloop = 1;
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

  // exit confirmation message
  printf("yay u left\n");
}

int main(void)
{
  prompt();
  return 0;
}
