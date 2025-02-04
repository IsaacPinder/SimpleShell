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
