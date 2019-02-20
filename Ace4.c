#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** parse(char input[512], char *tokenisedArray[512]);
void exCommand(char *tokenisedArray[512]);


int main() {
  char *tokenisedArray[512];
  char input [512];

  while (1) {

        printf(">");
        fgets(input,512,stdin);
        parse(input, tokenisedArray);
        if ((strcmp("exit\n", tokenisedArray[0]) == 0)) {
            break;
        }
        else if (getchar()==EOF) {
        	break;
        }
      
        exCommand(tokenisedArray);

    }
    return 0;
  }

//Parsing input function
char** parse(char input[512], char *tokenisedArray[512]) {

  int i=0;
	char* token;
	token = strtok(input, " |&<>;");
  tokenisedArray[0] = token;

	while (token != NULL) {
    i++;
		token = strtok(NULL, " |&<>;");
    tokenisedArray[i] = token;

	}
  return tokenisedArray;
}

void exCommand(char *tokenisedArray[512]) {

	pid_t pid = fork();
	int status;

	//fork returns a negative value: error has occured
	if (pid < 0) {
		perror("Error\n");
	}
	//fork returns 0: the child process is running
	else if (pid == 0) {
         if(execvp(tokenisedArray[0],tokenisedArray) == -1 ){
               printf("%s: Command not found\n",tokenisedArray[0]);
               exit(1);
         }

	}
	//parent process
	else {
		waitpid(pid, &status, 0);
		printf("Child Complete\n");
	}

}
