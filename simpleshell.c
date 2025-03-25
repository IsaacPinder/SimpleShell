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

// NOTES FOR QUALITY CHECK
// remove redundent commented code
// tidy up comments
// remove prints for testing
// (DONE) remove pre defined functions which we made (aslong as dostn break anything)
// tidy up comments(comment everything)
// tidy up what belongs in helpers v localfunctions

// QUESTIONS
// (DONE, TEST) do we have to free our allocated mallocs(that are saved to files, when we exit)?????? ( yes always)
// (DONE)when ctrl d pressed do add to history (no handeled before checked same time as checking less than 512, exit can be added but must be justified if it is or isnt)
// (DONE TEST)file paths????? for specefic machine .aliases??? .hist_list??? (make fieles definable use home directory insted of what weve got(hm dir /file) also change to histlist, snprintf for joining home directory and text file)
// is include 
//fix print hist

// STAGE1 FINAL TEST ?Is the input tokenized correctly (try ‘ls<tab>-lF;.&..>.<..|/<tab>fksdk’ 
//it should print an error for fksdk and the listings of . and .. twice plus the
// listing of / all formatted with entry per line and file permissions)?


void prompt()
{
  // int exit to for use in main loop to determine if we should continue running
  int exitloop = 0;

  // pointer to strings for use with strcmp to compare input to
  char *exitstr = "exit";
  char *getpathstr = "getpath";
  char *setpathstr = "setpath";
  char *cdstr = "cd";
  char *histstr = "history";
  char *execprevstr = "!!";
  char *addaliasstr = "alias";
  char *unaliasstr = "unalias";

  // array to hold 20 history items set items to null (so history is empty at start)
  char *history[20] = {NULL};

  // alias [i][0] = name alias[i][1] = command
  char *alias[10][2] = {{NULL}, {NULL}};
  // index of current command
  int commandIndex = 0;



  // save Home directory
  char *origDir = malloc(strlen(getenv("HOME"))+1);
  strcpy(origDir, getenv("HOME"));
  // print home and success status of changing to home
  printf("\nHomeDir : %s \n\n", origDir);
  printf(" chdir to home success? 0 good: %d \n\n", chdir(origDir));

  
  // load history and alias arrays from file
  commandIndex = getFromFile(history,origDir);
  loadAliasFile(alias,origDir);






  // malloc space for original path then copy the path into that space
  char *origPath = malloc(strlen(getenv("PATH"))+1);
  strcpy(origPath, getenv("PATH"));
  printf("Original_Path : %s \n\n", origPath);

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

    // if control d presssed dont try to tokenise or manipluate(comparisons)
    // non existant input(causes error) instead break from program loop
    if (line == NULL)
    {
      break;
    }

     // if input is not a history command add it to history
     if (line[0] != '!')
     {
       commandIndex = add_history(history, commandIndex, line);
       printf("added to hist\n");
     }

    // check if input is a alias
    // index for start navigating through line
    int charindex = 0;
    // index for navigating through first word of line
    int fwordindex = 0;
    char firstword[512];
    // skip over leading whitespace in line
    while (line[charindex] == ' ')
    {
      charindex++;
    }
    // copy charcters from line over to firstword until we reach end of first word(whitespace or \n or \0)
    while (line[charindex] != ' ' && line[charindex] != '\0' && line[charindex] != '\n')
    {
      firstword[fwordindex] = line[charindex];
      charindex++;
      fwordindex++;
    }
    // add end of string char to firstword
    firstword[fwordindex] = '\0';

    // loop through alias array
    for (int i = 0; i < 10; i++)
    {
      // if alias not empty(gap in array)
      if (alias[i][0] != NULL)
      {
        // if stored alias is the same as first word inputed
        if ((strcmp(alias[i][0], firstword) == 0))
        {

          // make a tempory copy of alias command (to not overwrite original)
          char aliascom[512];
          // copy original alias command to aliascom
          strcpy(aliascom, alias[i][1]);
          // concatenate aliascom to the rest of the line(from the end of first word)
          strcat(aliascom, &line[charindex]);
          // copy full line back into line (for tokenisation)
          strcpy(line, aliascom);

          break;
        }
      }
    }

    printf("first word %s \n", firstword);
    printf("line is %s \n", line);

    // tokenise input after it has been added to history
    tokenise(tokensarr, line);

    // handles history exec prev commands
    if (line[0] == '!')
    {
      // EXECUTE-PREVIOUS checking input isnt empty AND first token is "!!"
      if ((tokensarr[0] != NULL) && strcmp(tokensarr[0], execprevstr) == 0)
      {
        if (tokensarr[1] != NULL)
        {
          // too many arguments
          printf("ERROR there is too many arguments given, please amend your command\n");
        }
        else if (history[0] == NULL)
        {
          // no commands in history
          printf("ERROR there is no commands in history, please execute some commands\n");
        }
        else
        {
          char *histexecline = history[(commandIndex - 1) % 20];
          printf("current line to execute from hist %s \n", histexecline);
          tokenise(tokensarr, histexecline);
        }
      }
      // EXECUTE-COMMAND-NUM checking input isnt empty AND input is "!n" use n to execute comand
      else if ((tokensarr[0] != NULL) && str_exec_num(tokensarr[0], commandIndex, history) >= 0)
      {
        if (tokensarr[1] != NULL)
        {
          // too many arguments
          printf("ERROR there is too many arguments given, please amend your command\n");
        }
        else if (history[0] == NULL)
        {
          // no commands in history
          printf("ERROR there is no commands in history, please execute some commands\n");
        }
        else
        {
          // returns the number to execute
          int num_to_exec = str_exec_num(tokensarr[0], commandIndex, history);
          char *histexecline = history[num_to_exec];
          if (histexecline == NULL)
          {
            printf("History is empty at that index\n");
          }
          else
          {
            printf("current line to execute from hist %s \n", histexecline);
            tokenise(tokensarr, histexecline);
          }
        }
      }
      // EXECUTE-COMMAND-MINUS checking input isnt empty AND input is '!-n' use a subtracted n to execute comand
      else if ((tokensarr[0] != NULL) && str_exec_num_minus(tokensarr[0], commandIndex) >= 0)
      {
        if (tokensarr[1] != NULL)
        {
          // too many arguments
          printf("ERROR there is too many arguments given, please amend your command\n");
        }
        else if (history[0] == NULL)
        {
          // no commands in history
          printf("ERROR there is no commands in history, please execute some commands\n");
        }
        else
        {
          // returns the number to execute
          int num_to_exec = str_exec_num_minus(tokensarr[0], commandIndex);
          char *histexecline = history[num_to_exec];
          if (histexecline == NULL)
          {
            printf("History is empty at that index\n");
          }
          else
          {
            printf("current line to execute from hist %s \n", histexecline);
            tokenise(tokensarr, histexecline);
          }
        }
      }
    }

    // EXIT: User input is NOT empty AND first token is "exit" AND second is empty ∂then done looping
    if ((tokensarr[0] != NULL) && strcmp(tokensarr[0], exitstr) == 0 && tokensarr[1] == NULL)
    {
      exitloop = 1;
    }
    // GETPATH: checking input isn't empty AND first token is "getpath"
    else if ((tokensarr[0] != NULL) && strcmp(tokensarr[0], getpathstr) == 0)
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
    else if ((tokensarr[0] != NULL) && strcmp(tokensarr[0], setpathstr) == 0)
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
    else if ((tokensarr[0] != NULL) && strcmp(tokensarr[0], cdstr) == 0)
    {
      // if no arguments (2nd token empty) change to home directory
      if (tokensarr[1] == NULL)
      {
        // set directory to home directory
        chdir(origDir);
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
    // HISTORY checking input isnt empty AND first token is "history"
    else if ((tokensarr[0] != NULL) && strcmp(tokensarr[0], histstr) == 0)
    {
      if (tokensarr[1] != NULL)
      {
        // too many arguments
        printf("ERROR there is too many arguments given, please amend your command\n");
      }
      else
      {
        print_history(history, commandIndex);
      }
    }
    // ALIAS checking input isnt empty AND first token is "history"
    else if ((tokensarr[0] != NULL) && strcmp(tokensarr[0], addaliasstr) == 0)
    {
      // if only alias then call printAlias
      if (tokensarr[1] == NULL)
      {
        printAlias(alias);
      }
      // attempting to add alais not enough arguments
      else if (tokensarr[2] == NULL)
      {
        // not enough arguments
        printf("ERROR there is not enough arguments to add alias include a name and the command\n");
      }
      // else if 3rd token is not empty (too many arguments)
      else if (tokensarr[3] != NULL)
      {
        // too many arguments
        printf("ERROR there is too many arguments given, please amend your command\n");
      }
      // else add alias
      else
      {
        addToAlias(alias, tokensarr[1], tokensarr[2]);
      }
    }
    // UNALIAS checking input isnt empty AND first token is "history"
    else if ((tokensarr[0] != NULL) && strcmp(tokensarr[0], unaliasstr) == 0)
    {

      // not enough arguments
      if (tokensarr[1] == NULL)
      {
        // not enough arguments
        printf("ERROR there is not enough arguments to remove alias include the name\n");
      }
      // else if 3rd token is not empty (too many arguments)
      else if (tokensarr[2] != NULL)
      {
        // too many arguments
        printf("ERROR there is too many arguments given, please amend your command\n");
      }
      // else remove alias
      else
      {
        removeAlias(alias, tokensarr[1]);
      }
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

  // save history and alias array to file
  sendToFile(commandIndex, history, origDir);
  saveAliasToFile(alias,origDir);

  // set currentpath to orignalpath then free malloc
  set_path(origPath);
  printf("\npath set on exit: \n %s\n", get_path());
  free(origPath);
  origPath = NULL;

  // free malloc
  free(origDir);
  origDir = NULL;

  // exit confirmation message
  printf("yay u left\n");
}

int main(void)
{
  prompt();
  return 0;
}
