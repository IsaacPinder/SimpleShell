#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <unistd.h>    // fork, execl
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h>  // wait
#include <errno.h>     // errno

char * get_path(){
   
   return getenv("PATH");
   }
   
void set_path( char * Input){
     setenv("PATH",Input,1);
    }

void change_directory(char* dir){
 
  // run chdir if failed then print error 
       if(chdir(dir) == -1){
        perror("cd failed");
       }
}

void print_history(char *histarr[20], int commandIndex){
   //make copy of index
   // if theres nothing in array then just print a message saying so
   //loop through array printing all previouse things handeling circular array
   // e.g if go over edge see explanation on simpleshell.pdf and OUR(ISAAC AND SCOTT(DO BETTER GUYS)) pseudocode
 printf("fdsbhsdvbh");
}








