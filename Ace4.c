#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_ARGS 50

void get_input();
void get_path(char *tokenisedArray[MAX_ARGS]);
void set_path(char *tokenisedArray[MAX_ARGS]);
void changeDir(char *tokenisedArray[MAX_ARGS]);
void pwd();
void execute_command(char *tokenisedArray[MAX_ARGS]);
void writehistory(char* filename, char *history[20]);
int histcheck(char input[512], int historycounter, char *history[20]);
void history(char *tokenisedArray[MAX_ARGS], char *history[], int historycounter);

int main() {

    get_input();

    return 0;
  }

void get_input( ) {

    char *filename;
    FILE *fileread;
    char *history[20] = {NULL};
    int historycounter = 0;
    char cwd[256];
    char delimiters[] = " \t\n|<>&;";
    char *curPath = getenv("PATH");
    char *getHome = getenv("HOME");
    char input[512];

    if (chdir(getHome)==0){
      printf("Path set to Home.\n");
    }
    else{
      printf("Error setting home directory\n");
    }

    /* Checks for file called .hist_list */
    filename = strcat(getcwd(cwd,sizeof(cwd)),"/.hist_list");
    fileread = fopen(filename,"r");
    printf("Initial File Read is: %p\n",fileread);
    if(fileread != NULL)
    {
        char line [512];
	      char ch[512];
        while((fgets(line,sizeof line,fileread) != NULL) && (historycounter < 20))
        {
            history[historycounter] = strdup(strtok(line,"\n"));
            historycounter++;
		printf("HistoryList file is :%s\n",history[historycounter]);
        }

        fclose(fileread);
    }
    /* informs user if no file exists. */
    else
    {
        printf("No history exists. A new history will be created upon exit. \n\n");
    }

    while(1) {
        pwd();
        printf(">");
        char *inputLine = fgets(input,512,stdin);
        input[strcspn(input,"\n")] = '\0';
        fflush(stdin);
        if ((strcmp("exit", input) == 0) || inputLine == NULL) {
            break;
        }
        // tokenises input
        int i=0;
        char *token;
        char *tokenisedArray[MAX_ARGS];
        token = strtok(input, delimiters);
        //tokenisedArray[0] = token;
        while (token != NULL) {
           tokenisedArray[i++] = token;
           token = strtok(NULL, delimiters);
        }
        char *null = NULL;
        tokenisedArray[i++] = null;
        // end of tokenising

        execute_command(tokenisedArray);
    }
    writehistory(filename, history);
    printf("Shutting down shell...\n");
    setenv("PATH", curPath, 1);
    printf("Path on shut down : %s\n", getenv("PATH"));
    printf("Goodbye.\n");
}

void get_path(char *tokenisedArray[MAX_ARGS]) {

  if (tokenisedArray[1]!=NULL){
    printf("getpath function does not have parameters\n");
  }
  else{
      printf("Current path is : %s\n", getenv("PATH"));
  }
}

void set_path(char *tokenisedArray[MAX_ARGS]) {

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

void changeDir(char *tokenisedArray[MAX_ARGS]){
	if (tokenisedArray[2] == NULL){
		if (tokenisedArray[1] == NULL){
			if (chdir(getenv("HOME")) != 0){
				perror("Error");
			}
		}
		else{
			if (chdir(tokenisedArray[1]) != 0){
				perror("Error");
			}
		}
	}
	else{
		printf("cd: too many arguments\n");
	}
	return;
}

void pwd(){
	char cwd[512];
	getcwd(cwd, 512);
	printf("%s", cwd);
}

void execute_command(char *tokenisedArray[MAX_ARGS]) {

  if (strcmp("getpath", tokenisedArray[0])==0){
    get_path(tokenisedArray);
  }
  else if (strcmp("setpath", tokenisedArray[0])==0){
    set_path(tokenisedArray);
  }
  else if(strcmp("cd", tokenisedArray[0])==0)
  {
    changeDir(tokenisedArray);
  }
  else {

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
                    perror("");
               }
               exit(0);

        }
        //parent process
        else {
          int status;
          waitpid(pid, &status, 0);
        }
  }


}
/* This function writes the history to a specified text file upon termination of the program. */
void writehistory(char* filename, char *history[20])
{
    FILE *filewrite;
    int i;
    //filename = strcat(filename,"/.hist_list");
    printf("Current filename in writeHistory() is: %p\n\n",filename);
    filewrite = fopen ( filename, "w" );
    printf("filewrite in writeHistory() is: %p\n\n", filewrite);
    if (filewrite == NULL)
    {
        /* Lets user know if file fails to open .*/
        printf("Error opening file!\n");
        exit(1);
    }
    i = 0;

    while(history[i] != 0)
    {
        /* Write each line of history into text file and close reader upon completion.*/
        fprintf(filewrite,"%s\n",history[i]);
        i++;
        if(i == 20)
        {
            break;
        }
    }
    fclose ( filewrite );
}

/* This function checks user input against the history and handles "!!" and "!<no>" commands used to run previous commands from the history. */
int histcheck(char input[512], int historycounter, char *history[20])
{

    /* Prevents line breaks from being included in the history. */
    if(strstr(input,"\n"))
    {}
    else
    {
        /* If the program passes this if statement, the first char in the input is a ! or the input is "history" */
        if((strncmp("!", input, 1) != 0) && (strncmp("history", input, 512) != 0 ))
        {

            /* When array is full, move each element in the array down one location. */
            if(historycounter == 20)
            {
                int i = 0;
                while(i < (historycounter - 1))
                {
                    history[i] = history[i + 1];
                    i++;
                }
                /* Add the new element to history. */
                history[(20 - 1 )] = strdup(input);
            }
            else
            {
                /* Add the new element to history straight away. Then increment counter */
                history[historycounter] = strdup(input);
                historycounter = historycounter + 1;
            }
        }
        else
        {
            /* Checks if input is to run the last command. (!!) If so, then execute it,
             or display an error if there is no history to run. */
            if(strncmp("!!", input, 512) == 0)
            {
                if(history[0] != 0)
                {
                    if(historycounter == 0)
                    {
                        strcpy(input,history[19]);
                    }
                    else
                    {
                        strcpy(input,history[historycounter - 1]);
                    }
                }
                else
                {
                    printf("There is no history to run\n");
                }
            }
            else
            {
                long length = strlen(input);
                if (length > 1)
                {
                    int location = atoi(&input[1]); /* Converts char to int. */
                    if(location > 0 && location < 21)
                    {
                        location = location - 1;
                        if(history[location] != NULL)
                        {
                            /* Because the history command prints out from most to last recent, this converts the users
                             input into an array location, as !1 would be location in an full array. */
                            if((historycounter-location) < 0)
                            {
                                location = (20 - (historycounter - (location + 1)));
                            }
                            else
                            {
                                location = (historycounter - (location + 1));
                            }
                            /* The command from history is copied into input overwriting the previous command.
                             The command is displayed before it runs. */
                            strcpy(input,history[location]);
                            printf("%s\n",input);
                        }
                    }
                    else
                    {
                        if(location != 0)
                        {
                            printf("The character following ! must be a number between 1 and 20 to execute a previous command.\n");
                        }
                    }
                }
            }
        }
    }

    return historycounter;
}

/* History displays to the user the last 20 commands they entered with numbering next to each element.
 By typing ! and the number in the form !<no> the user can execute these commands. This function
 however exists solely to display those commands to the user. */
void history(char *tokenisedArray[MAX_ARGS], char *history[], int historycounter)
{
    int j = 0;
    if(tokenisedArray[1] == 0)
    {
        if(history[0] == 0)
        {
            printf("History is empty\n");
        }
        else
        {
            /* This for loop finds out where the latest addition to the array is. */
            int k = 0;
            while(history[k] != 0)
            {
                k++;
                if(k == (20))
                {
                    break;
                }
            }

            /* Prints out the recent history, with the most recent command as element 1. Less recent as incrementations of 1. */
            while(history[j] != 0)
            {
                printf("%d. %s\n",(k),history[j]);
                j++;
                k--;
                if(j == 20)
                {
                    break;
                }
            }
        }
    }
    else
    {
        /*If history is given parameters it will not execute
         and instead the user is prompted not to give such input */
        printf("History does not take parameters\n");
    }
    return;
}
