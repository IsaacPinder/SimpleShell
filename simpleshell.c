#include <stdio.h>
#include <string.h>
// ** Commands for compiling and running **
// gcc simpleshell.c -pedantic -Wall -o simpleshell
/// simpleshell

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
      printf("%s ", token);
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
