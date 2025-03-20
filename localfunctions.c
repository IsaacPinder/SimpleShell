#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <unistd.h>    // fork, execl
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h>  // wait
#include <errno.h>     // errno

char *get_path()
{

    return getenv("PATH");
}

void set_path(char *Input)
{
    setenv("PATH", Input, 1);
}

void change_directory(char *dir)
{

    // run chdir if failed then print error
    if (chdir(dir) == -1)
    {
        perror("cd failed");
    }
}

// coded by Paul
void print_history(char *histarr[20], int commandIndex)
{
    // check if history is empty
    if (histarr[0] == NULL)
    {
        printf("No commands in history.\n");
        return;
    }

    int start = (commandIndex >= 20) ? commandIndex % 20 : 0;

    if (histarr[19] != NULL && commandIndex < 19)
    {
        int count = 0;

        for (int i = commandIndex; i < 20; i++)
        {

            printf("%d: %s \n", count, histarr[i]); // printing history based on the index
            count++;

        }
        printf("\n");
        for (int i = 0; i < commandIndex; i++)
        {
            printf("%d: %s \n", count, histarr[i]); // printing history based on the index
            count++;
        }
    }
    else
    {
        
        for (int i = 0; i < 20; i++)
        {
            int index = (start + i) % 20;
            if (histarr[index] != NULL)
            {
                printf("%d: %s \n", index, histarr[index]); // printing history based on the index
            }
        }
        if (histarr[0] == NULL)
        {
            printf("No commands in history.\n");
            return;
        }

        // int start = (commandIndex >= 20) ? commandIndex % 20 : 0;

        if (histarr[19] != NULL && commandIndex < 19)
        {
            int count = 0;

            for (int i = commandIndex; i < 20; i++)
            {

                printf("%d: %s \n", count, histarr[commandIndex]); // printing history based on the index
                count++;

            }

            for (int i = 0; i < commandIndex; i++)
            {
                printf("%d: %s \n", count, histarr[i]); // printing history based on the index
                count++;
            }
        }
    }

    
}
