#include <stdio.h>
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
  int exit = 0;
  // pointer to string for use with str_cmp to compare input to "exit"
  char *exitstr = "exit\n";

  // main loop continue while exit != 1
  while (exit == 0)
  {
    printf("£: ");
    // buffer to take in input
    char input[512];
    // take in user input
    char *line = fgets(input, sizeof(input), stdin);

    // tolkenises the line string by provided delimeters e.g (" \t|><&;")
    char *token = strtok(line, " \t|><&;");

    // loops through line string breaking it into smaller strings until end of line
    while (token != NULL)
    {
      // this print shows tolkens
      // printf("%s", token);
      token = strtok(NULL, " \t|><&;");
    }

    // trim whitespace from input
    char *trminput = str_trim(input);
    // if User input is "exit" or ^D is pressed(therefor line is NULL)then done looping
    if (str_cmp(trminput, exitstr) == 0 || line == NULL)
    {
      exit = 1;
    }

    // else ask operating system for command
    else
    {

      // fork
      pid_t p = fork();

      // if < 0 then error happened
      if (p < 0){
        printf("Now errno = %d\n", errno);
      }
      // child process
      else if (0 == p)
      {
        // execvp expects contant argument for token
        execvp("simpleshell.c", token);
        printf("Now errno = %d\n", errno);
      
        
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
