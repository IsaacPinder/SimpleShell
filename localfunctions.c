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
    // returns current file path
    return getenv("PATH");
}

void set_path(char *Input)
{
    // sets file path
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


void print_history(char *histarr[20], int commandIndex) {
    //check if history is empty
    if (histarr[0] == NULL) {
        printf("No commands in history.\n");
        return;
    }

    int count = 1;

    //checking history has wrapped around
    if (histarr[19] != NULL && commandIndex < 19) {
        for (int tail = commandIndex; tail < 20; tail++) {
            printf("%d: %s\n", count++, histarr[tail]);
        }
        for (int start = 0; start < commandIndex; start++) {
            printf("%d: %s\n", count++, histarr[start]);
        }
    } else { 
        //for loop like normal if history hasn't wrapped around
        for (int i = 0; i < 20; i++) {
            int index = (commandIndex + i) % 20;
            //making sure it isnt printing null value
            if (histarr[index] != NULL) {
                printf("%d: %s\n", count++, histarr[index]);
            }
        }
    }
}
