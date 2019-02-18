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
  bool userExit=false;
  char input [512];

  do {
    printf(">");
    fgets(input,512,stdin);
    parse(input, tokenisedArray);
    exCommand(tokenisedArray);
    if (strcmp("exit", tokenisedArray[0])){
      break;
    }
  } while(userExit==false);

	return 0;
}

//Display Prompt function
// void display_prompt() {
//
// 	char input[512];
//
//
//
//
// 	printf("%s", input);
// 	parse(input);

	//if (*input=="exit") {
	//	printf("Identical");
	//}
	//else {
	//	printf("Not identical");
	//}

//}

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
		perror("Error");
		exit(1);
	}
	//fork returns 0: the child process is running
	else if (pid == 0) {
    if(execvp(tokenisedArray[0],tokenisedArray) == -1 ){
               printf("%s: Command not found\n",tokenisedArray[0]);
      }
      exit(2);
	}
	//parent process
	else {
		waitpid(pid, &status, 0);
		printf("Child Complete");
	}

}
