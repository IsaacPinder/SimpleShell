#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

char * get_path(){
   char* temppath = getenv("PATH");
   
   return temppath;
   }
   
   void set_path( char * Input){
     setenv("PATH",Input,1);
    }
