void prompt();
int str_len(char *str);
int str_cmp(char *str1, char *str2);
int whitespace(char c);
char *str_trim(char *str);
char *get_path();
void set_path(char *Input);
void change_directory(char* dir);
void print_history(char *histarr[20], int commandIndex);
int str_exec_num(char* input1,int index , char* histarr[]);
int str_exec_num_minus(char* input1, int index);
int add_history(char *histarr[20], int commandIndex, char* command);
 void tokenise(char* tokensarr[],char* line);

