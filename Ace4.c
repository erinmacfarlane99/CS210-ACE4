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
// void exCommand(char cmd[30], char *tokenisedArray[MAX_ARGS]);


int main() {
  char *tokenisedArray[MAX_ARGS];
  char input[512];
  char cmd[30];
  char *delimiters = " \t\n|<>&;";

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
        if ((strcmp("exit", tokenisedArray[0]) == 0) || getchar()==EOF) {
            break;
        }
        // executes command
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
      //  exCommand(cmd, tokenisedArray);
    }

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

// void exCommand(char cmd[30], char *tokenisedArray[MAX_ARGS]) {
//
// 	pid_t pid = fork();
//
//   printf("%s\n", cmd);
// 	//fork returns a negative value: error has occured
// 	if (pid == -1) {
// 		perror("Error\n");
//     exit(1);
// 	}
//
// 	//fork returns 0: the child process is running
// 	else if (pid == 0) {
//          if(execvp(tokenisedArray[0],tokenisedArray) == -1 ){
//                printf("%s: Command not found\n",tokenisedArray[0]);
//
//          }
//
// 	}
// 	//parent process
// 	else {
//     int status;
// 		waitpid(pid, &status, 0);
// 		printf("Child Complete\n");
// 	}
//
// }
