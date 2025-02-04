#include <stdio.h>
#include <string.h>
#include "header.h"

// ** Commands for compiling and running **
// gcc simpleshell.c -pedantic -Wall -o simpleshell
/// simpleshell

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

    // if User input is "exit" or ^D is pressed(therefor line is NULL)then done looping
    if (str_cmp(input, exitstr) == 0 || line == NULL)
    {
      exit = 1;
    }

    // tolkenises the line string by provided delimeters e.g (" \t|><&;")
    char *token = strtok(line, " \t|><&;");

    // loops through line string breaking it into smaller strings until end of line
    while (token != NULL)
    {
      // this print f causes issue with spacing of second £: onwards
      printf("%s", token);
      token = strtok(NULL, " \t|><&;");
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
