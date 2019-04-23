// Siddharth Rajan
// Arslan Saeed
// Ishraq Khan
// Raymund Rodriguez
// CSC 33200 Shell Project

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void clr(char* input, char* c, char** args, int n, int fL) {
	if (args) {
		for (int i = 0; i < n; ++i) {
			if (args[i])
				free(args[i]);
			args[i] = NULL;
		}
	}
	if (input) 
		free(input);
	if (args && fL)
		free(args);
	if (c)
		free(c);
}

int main() {
    int j = 0;
	int status;
	char* ptr = NULL;
	char* tmp = NULL;
	char* input = (char*) calloc(1025, sizeof(char));
	char* cwd = (char*) calloc(1024, sizeof(char));
	char** args = (char**) calloc(1024 / 64, sizeof(char*));

    getcwd(cwd, 1024);
    printf("%s\n", cwd);

    printf("! ");

    while (fgets(input, 1024, stdin)) {
        input[strlen(input) - 1] = '\0';
        tmp = input;
        if (strcmp(input, "\0") == 0) {		// If user entered nothing
			printf("! ");
            continue;
        }

		for (j = 0, ptr = strtok(tmp, " "); ptr; ptr = strtok(NULL, " "), j++) {	// Take the user input and put each word in an array
			args[j] = (char*)calloc(65, sizeof(char));
			strcpy(args[j], ptr);
		}

        if (strcmp(args[0], "EXIT*") == 0) {    // If user entered "EXIT*"
            char ch = 'f'; 

            printf("Group members: \n");
            printf("Arslan Saeed\n");
            printf("Ishraq Khan\n");
            printf("Raymund Rodriguez\n");
            printf("Siddharth Rajan\n");
            printf("Press enter to exit: \n"); 

            while(ch != '\n') {
                scanf("%c", &ch);
            }
        
        break;
        }
			
        else if (strcmp(args[0], "cd*") == 0) {	    // If user entered "cd*"
			chdir((args[1]) ? args[1] : "..");      // Change directory to whatever the user typed or the parent directory
			for (int i = 0; i < j; ++i) {
				if (args[i])
					free(args[i]);
				args[i] = NULL;
			}
            memset(cwd, '\0', 1024);
            getcwd(cwd, 1024);              
            printf("%s\n", cwd);            // Get new current path/directory and print it
			printf("! ");
			continue;
		}

        else if (strcmp(args[0], "shell") == 0) {   // If user entered "shell"
            // Handle case for shell here
        }

        else if (strcmp(args[0], "History*") == 0) {    // If user entered "History*"
            // Handle case for History* here
        }

        else if (strcmp(args[0], "clear*") == 0) {      // If user entered "clear*"
            // Handle case for clear* here
        }

        else {                                          // If user entered anything else
            int child_exec = fork();                    // Fork a child process to handle it
            if (child_exec == 0) {
                if (execvp(args[0], args) == -1)        // Call execvp() to handle it
				    perror("ERROR");
                clr(input, cwd, args, j, 1);
			    exit(0);
            }
            else if (child_exec > 0) {                  // The parent waits for the child to finish
                waitpid(child_exec, &status, 0);
			    clr(NULL, NULL, args, j, 0);
            }
        }

        printf("! ");
    }

    return 0;
}