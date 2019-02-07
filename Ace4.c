#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void display_prompt();
void parse(char input[512]);
void exCommand();


int main() {

	display_prompt();
	exCommand();

	return 0;
}

//Display Prompt function
void display_prompt() {
	
	char input[512];
	

	printf(">");
	fgets(input,512,stdin);
	
	printf("%s", input);	
	parse(input);
	
	//if (*input=="exit") {
	//	printf("Identical");
	//}
	//else {
	//	printf("Not identical");
	//}
	
}

//Parsing input function
void parse(char input[512]) {

	char* token;
	char* token2;
	token = strtok(input, " |&<>;");

	while (token != NULL) {
		printf("%s\n", token);
		token = strtok(NULL, " |&<>;");
	}
}

void exCommand() {

	pid_t pid = fork();
	int status;

	//fork returns a negative value: error has occured
	if (pid < 0) { 
		perror("Error");
		exit(1);
	}
	//fork returns 0: the child process is running
	else if (pid == 0) { 
		execvp(*argv, argv);
	}
	//parent process
	else { 
		wait(&status);
		printf("Child Complete");
	}

}