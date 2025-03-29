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
// (DONE)tidy up comments(comment everything)
// tidy up what belongs in helpers v localfunctions

// QUESTIONS
// (DONE, TEST) do we have to free our allocated mallocs(that are saved to files, when we exit)?????? ( yes always)
// (DONE)when ctrl d pressed do add to history (no handeled before checked same time as checking less than 512, exit can be added but must be justified if it is or isnt)
// (DONE TEST)file paths????? for specefic machine .aliases??? .hist_list??? (make fieles definable use home directory insted of what weve got(hm dir /file) also change to histlist, snprintf for joining home directory and text file)
// is include
// fix print hist

// STAGE1 FINAL TEST ?Is the input tokenized correctly (try ‘ls<tab>-lF;.&..>.<..|/<tab>fksdk’
// it should print an error for fksdk and the listings of . and .. twice plus the
// listing of / all formatted with entry per line and file permissions)?

int main(void)
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
  // index of current command in history
  int commandIndex = 0;

  // malloc space for home directory then copy home directory into that space
  char *origDir = malloc(strlen(getenv("HOME")) + 1);
  strcpy(origDir, getenv("HOME"));

  // print home and success status of changing to home
  printf("\nHomeDir : %s \n\n", origDir);
  printf(" chdir to home success? 0 good: %d \n\n", chdir(origDir));

  // load history and alias arrays from file
  commandIndex = getFromFile(history, origDir);
  loadAliasFile(alias, origDir);

  // malloc space for original path then copy the path into that space
  char *origPath = malloc(strlen(getenv("PATH")) + 1);
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

    // if input is not a history command or a newlineadd it to history
    if (line[0] != '!' && line[0] != '\n')
    {
      commandIndex = add_history(history, commandIndex, line);
    }

    // tokenise input after it has been added to history
    tokenise(tokensarr, line);

    // dont try to invoke an alias if user is trying to add or remove an alias
    if (strcmp(tokensarr[0], "alias") != 0 && strcmp(tokensarr[0], "unalias") != 0)
    {

      // loop through alias list
      for (int i = 0; i < 10; i++)
      {
        // loop for all tokens
        for (int j = 0; j < 50 && tokensarr[j] != NULL; j++)
        {

          // if alias not empty(not gap in array)
          if (alias[i][0] != NULL)
          {
            // if stored alias is the same as a Token inputed
            if ((strcmp(alias[i][0], tokensarr[j]) == 0))
            {

              // copy alias command into the relevent token overwriting it
              strcpy(tokensarr[j], alias[i][1]);
            }
          }
        }
      }

      // once we have replaced a token with its relevent alias command we have to retokenise
      // because by replacing a token with alias command we could replace a singular token with an untokenised line
      // e.g token[5] = "ls -a" therefor for the program to execute this must be broken into seperate "ls" "-a" tokens
      // so we concatenate everything into one line and retokensie

      // temp variable for concatenating every token
      char temp[512] = "";
      // loop through every token
      for (int i = 0; i < 50 && tokensarr[i] != NULL; i++)
      {
        // Add space between words if not first word
        if (i > 0)
        {
          strcat(temp, " ");
        }

        // concatenate every token into temp string
        strcat(temp, tokensarr[i]);
      }

      // once all tokens have been concatented into temp line retokenise temp linne
      tokenise(tokensarr, temp);
    }

    // handles history commands this ensures that wont try and run a history command and normal command
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

          // use temp copy of histexecline so that we dont alter(tokenise) the original array
          char temp[512];
          strcpy(temp, histexecline);
          tokenise(tokensarr, temp);
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

            // use temp copy of histexecline so that we dont alter(tokenise) the original array
            char temp[512];
            strcpy(temp, histexecline);
            tokenise(tokensarr, temp);
          }
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
          // minus one to match with the printing starting from 1 instead of 0
          char *histexecline = history[(num_to_exec - 1)];
          if (histexecline == NULL)
          {
            printf("History is empty at that index\n");
          }
          else
          {
            printf("current line to execute from hist %s \n", histexecline);

            // use temp copy of histexecline so that we dont alter(tokenise) the originasl array
            char temp[512];
            strcpy(temp, histexecline);
            tokenise(tokensarr, temp);
          }
        }
      }
    }

    // EXIT: User input is NOT empty AND first token is "exit" AND second is empty then done looping
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
    // ALIAS checking input isnt empty AND first token is "alias"
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
      else
      {

        // to be able to accept multiple parameters for the aliased command
        //loop through tokens and concatnete them into one line
        
        // temp variable for concatenating tokens into
        char temp[512] = "";
        // loop for all tokens (skipping first 2 cos (alias name ...)
        for (int i = 2; i < 50 && tokensarr[i] != NULL; i++)
        {

          // Add space between words
          if (i > 2)
          {
            strcat(temp, " ");
          }

          // concatenate every token into one line
          strcat(temp, tokensarr[i]);
        }
        // add concatenated line to alias as command
        addToAlias(alias, tokensarr[1], temp);
      }
    }
    // UNALIAS checking input isnt empty AND first token is "unalias"
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
  saveAliasToFile(alias, origDir);

  // set currentpath to orignalpath then free malloc
  set_path(origPath);
  printf("\npath set on exit: \n %s\n", get_path());
  free(origPath);
  origPath = NULL;

  // free malloc for dir
  free(origDir);
  origDir = NULL;

  // exit confirmation message
  printf("Shell exiting\n");
}
