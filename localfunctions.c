#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

char * get_path(){
   
   return getenv("PATH");
   }
   
void set_path( char * Input){
     setenv("PATH",Input,1);
    }
