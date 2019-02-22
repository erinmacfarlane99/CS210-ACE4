#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_ARGS 50

// char** parse(char input[512], char cmd[30], char *tokenisedArray[MAX_ARGS]);
void get_path(char *tokenisedArray[MAX_ARGS]);
void get_path(char *tokenisedArray[MAX_ARGS]);
void exCommand(char *tokenisedArray[MAX_ARGS]);



int main() {
  char *tokenisedArray[MAX_ARGS];
  char input[512];
  char *delimiters = " \t\n|<>&;";
  char *curPath = getenv("PATH");
  printf("%s\n", curPath);
  char *getHome = getenv("HOME");

  if (chdir(getHome)==0){
    printf("Path set to Home.\n");
  }
  else{
    printf("Error setting home directory\n");
  }


  while (1) {

        printf(">");
        fgets(input,512,stdin);
        // tokenises input
        int i=0;
        char* token;

        token = strtok(input, delimiters);
        tokenisedArray[0] = token;
        while (token != NULL) {
          i++;
          token = strtok(NULL, delimiters);
          tokenisedArray[i] = token;

        }
        // end of tokenising
        if ((strcmp("exit", tokenisedArray[0]) == 0)) {
            break;
        }
        else if (getchar()==EOF){
          break;
        }
        // executes command

        exCommand(tokenisedArray);
    }
    printf("Shutting down shell...\n");
    setenv("PATH", curPath, 1);
    printf("Path on shut down : %s\n", getenv("PATH"));
    printf("Goodbye.\n");

    return 0;
  }

//Parsing input function
// char** parse(char input[512], char cmd[30], char *tokenisedArray[MAX_ARGS]) {

//   int i=0;
// 	char* token;
// 	token = strtok(input, " |&<>;");
//   strcpy(cmd, token);
//   printf("%s\n", cmd);
// 	while (input != NULL && i<50) {
// 		token = strtok(NULL, " |&<>;");
//     tokenisedArray[i] = token;
//     i++;
// 	}
//   return tokenisedArray;
// }

void get_path(char *tokenisedArray[MAX_ARGS]){
  if (tokenisedArray[1]!=NULL){
    printf("getpath function does not have parameters\n");
  }
  else{
      printf("Current path is : %s\n", getenv("PATH"));
  }

}

void set_path(char *tokenisedArray[MAX_ARGS]){

  if (tokenisedArray[1]!=NULL){
    if (tokenisedArray[2]!=NULL){
      printf("Too many parameters!\n");
    }
    else{
      if (setenv("PATH", tokenisedArray[1], 1)==0){
        printf("Path set successfully.\n");
      }
      else{
        printf("Path doesn't exist.\n");
      }
    }
  }
  else{
    printf("Path needs to be included.\n");
  }
}

void exCommand(char *tokenisedArray[MAX_ARGS]) {

  if (strcmp("getpath", tokenisedArray[0])==0){
    get_path(tokenisedArray);
  }
  else if (strcmp("setpath", tokenisedArray[0])==0){
    set_path(tokenisedArray);
  }
  else{
    pid_t pid = fork();
  	//fork returns a negative value: error has occured
  	if (pid == -1) {
  		perror("Error\n");
      exit(1);
  	}
  	//fork returns 0: the child process is running
  	else if (pid == 0) {
           if(execvp(tokenisedArray[0],tokenisedArray) == -1 ){
                 printf("%s: Command not found\n",tokenisedArray[0]);

           }

  	}
  	//parent process
  	else {
      int status;
  		waitpid(pid, &status, 0);
  		printf("Child Complete\n");
  	}
  }


}
