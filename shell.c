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
#include <dirent.h>

#define HISTORY_SIZE 5 // Size of history array

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

// Returns the max index that the history array can go up to
int getHistoryBound(int hTimes) {
	if (hTimes > HISTORY_SIZE) {
		return HISTORY_SIZE;
	}

	return hTimes;
}

// Frees memory throughout the history array
void freeHistory(char** history, int hTimes) {
	int end = getHistoryBound(hTimes);

	for (int i = 0; i < HISTORY_SIZE; i++) {
		free(history[i]);
	}
}

// Prints the contents of history array
void printHistory(char** history, int hTimes) {
	int end = getHistoryBound(hTimes);
	printf("\n");
	for (int i = 0; i < end; i++) {
		printf("\t%d\t%s\n", hTimes - HISTORY_SIZE + i, history[i]);
	}

	printf("\n");
}


// Returns an array with the current input inserted in to the history array
void updateHistory(char* history[], char* input, int* hTimes) {
	int i = *hTimes;

	if (*hTimes >= HISTORY_SIZE) {
		for (i = 0; i < HISTORY_SIZE - 1; i++) {
			history[i] = malloc(HISTORY_SIZE * sizeof(char*));
			strcpy(history[i], history[i + 1]);
		}
	}

	history[i] = malloc(HISTORY_SIZE * sizeof(char*));
	strcpy(history[i], input);

	*hTimes += 1;
}

// Clears terminal by using ANSI escape sequences
void clearTerminal() {
	printf("\033[2J"); // erase display
	printf("\033[1;1H"); // position cursor at home position
}


// Lists current files in directory specified by cwd
void list(char* dir) {
	struct dirent *dirptr = NULL;
	DIR *cwd = opendir(dir);
	if (cwd == NULL) {
		perror("Error: ");
	}
	printf("\n");

	while ((dirptr = readdir(cwd)) != NULL) {	// Iterate over opened directory and print contents
		if (dirptr->d_name[0] != '.') {
			printf("%s\n", dirptr->d_name);
		}
	}
	printf("\n");
}

int main() {
	int j = 0;
	int status;
	char* ptr = NULL;
	char* tmp = NULL;
	char* input = (char*) calloc(1025, sizeof(char));
	char* cwd = (char*) calloc(1024, sizeof(char));
	char** args = (char**) calloc(1024 / 64, sizeof(char*));

	char* history[HISTORY_SIZE]; // Array of last 5 commands entered
	int hTimes = 0; // Tracks how many times history was called

	getcwd(cwd, 1024);
	printf("\nYou are at %s\n\n", cwd);

	printf("! ");

	while (fgets(input, 1024, stdin)) {
		input[strlen(input) - 1] = '\0';
		tmp = input;
		if (strcmp(input, "\0") == 0) {		// If user entered nothing
			printf("! ");
			continue;
		}

		updateHistory(history, input, &hTimes);

		for (j = 0, ptr = strtok(tmp, " "); ptr; ptr = strtok(NULL, " "), j++) {	// Take the user input and put each word in an array
			args[j] = (char*)calloc(65, sizeof(char));
			strcpy(args[j], ptr);
		}

		if (strcmp(args[0], "Exit*") == 0) {    // If user entered "EXIT*"
			char ch = 'f';

			printf("\nGroup members: \n");
			printf("\tArslan Saeed\n");
			printf("\tIshraq Khan\n");
			printf("\tRaymund Rodriguez\n");
			printf("\tSiddharth Rajan\n");
			printf("\nPress enter to exit: ");

			while(ch != '\n') {
				scanf("%c", &ch);
			}
			printf("\n");

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
			printf("\nYou are at %s\n\n", cwd);            // Get new current path/directory and print it
			printf("! ");
			continue;
		}

		else if (strcmp(args[0], "shell") == 0) {   // If user entered "shell"
			printf("\nCustom commands:\n");
			printf("\tshell: shows a list of custom commands (like this one)\n");
			printf("\tcd*: goes to the parent directory by default or any other directory and shows the current working directory\n");
			printf("\tHistory*: shows the last 5 executed commands\n");
			printf("\tclear*: clears the terminal screen and shows the contents of the current directory\n");
			printf("\tExit*: shows the names of the people who made this shell and waits for the user to press \'Enter\' to quit it\n\n");
		}

		else if (strcmp(args[0], "History*") == 0) {    // If user entered "History*"
			// Handle case for History* here
			printHistory(history, hTimes);
		}

		else if (strcmp(args[0], "clear*") == 0) {      // If user entered "clear*"
			// Handle case for clear* here
			clearTerminal();
			list(cwd);
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

	freeHistory(history, hTimes);

	return 0;
}
