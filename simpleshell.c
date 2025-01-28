# include <stdio.h>
# include <string.h>
 
int str_len(char* str){
int valid = 0;
int count = 0;
while (!valid){
if (*str == '\0'){
valid = 1;
}
else{
count ++;
str ++;
}
}
return count;
}

 int str_cmp(char *str1, char *str2){
for(int i = 0; i < str_len(str1);i++){

if (*(str1 + i) > *(str2 + i)){
return 1;
}

if (*(str1 + i) < *(str2 + i)){
return -1;
}
}

return 0;

}





 void prompt(){
  int exit = 0;
  char *exitstr = "exit\n";

    while(exit == 0){
      printf("£: ");
      
      char input[512];
      
     char* line = fgets(input,sizeof(input),stdin);
    
      if(str_cmp(input,exitstr) == 0||line == NULL){
        exit = 1;
        
      }
      
     char* token = strtok(line," \t|><&;");
      
      while(token != NULL){
      printf("%s ",token);
      token = strtok(NULL," \t|><&;");
      }
      
    }
   printf("yay u left\n");
 }
 
 
 
 
 
 int main(void) {
   prompt();
   return 0;
}
 
