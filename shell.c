#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define TRUE 1
// Descrição de algumas funções utilizadas no código:	
//  pathname must be either a binary executable, or a script starting with a line of the form:
// #!interpreter[optional - arg]

//  int execve(const char *pathname, char *const argv[], char *const envp[]);

//  argv  is  an  array  of argument strings passed to the new program.  By convention, the first of these strings (i.e., argv[0]) should contain the filename associated with the file being executed.

// int stat(const char *path, struct stat *buf);
// The stat() function obtains information about the named file and writes it to the area pointed to by the buf argument.The path argument points to a pathname naming a file.
// Upon successful completion, 0 is returned. Otherwise, -1 is returned and errno is set to indicate the error.

// Alunos:
// Guilherme Leite Félix
// Diego Sindeaux Soares de Holanda

char command[100];
char *parameters[20];
int status;

void type_prompt() {
    printf("simple-shell$ ");
}

void read_command() {
    char *token;
    int i = 0;
    fgets(command, 100, stdin);
    token = strtok(command, " \n");
    while (token != NULL) {
        parameters[i] = token;
        token = strtok(NULL, " \n");
        i++;
    }
    parameters[i] = NULL;
}

char *get_location(char *command) {
    char *path, *path_copy, *path_token, *file_path;
    int command_length, directory_length;
    struct stat buffer;
    path = getenv("PATH");
    if (path) {
        /* Duplicate the path string -> remember to free up memory for this because strdup allocates memory that needs to be freed*/
        path_copy = strdup(path);
        /* Get length of the command that was passed */
        command_length = strlen(command);
        /* Let's break down the path variable and get all the directories available*/
        path_token = strtok(path_copy, ":");
        while (path_token != NULL) {
            /* Get the length of the directory*/
            directory_length = strlen(path_token);
            /* allocate memory for storing the command name together with the directory name */
            file_path = malloc(command_length + directory_length + 2); /* NB: we added 2 for the slash and null character we will introduce in the full command */
            /* to build the path for the command, let's copy the directory path and concatenate the command to it */
            strcpy(file_path, path_token);
            strcat(file_path, "/");
            strcat(file_path, command);
            strcat(file_path, "\0");
            /* let's test if this file path actually exists and return it if it does, otherwise try the next directory */
            if (stat(file_path, &buffer) == 0) {
                /* return value of 0 means success implying that the file_path is valid*/
                /* free up allocated memory before returning your file_path */
                free(path_copy);
                return (file_path);
            } else {
                /* free up the file_path memory so we can check for another path*/
                free(file_path);
                path_token = strtok(NULL, ":");
            }
        }
        /* if we don't get any file_path that exists for the command, we return NULL but we need to free up memory for path_copy */
        free(path_copy);
        /* before we exit without luck, let's see if the command itself is a file_path that exists */
        if (stat(command, &buffer) == 0){
            return (command);
        }
        return (NULL);
    }
    return (NULL);
}

int main (int argc, char* argv[]) {
    while (TRUE){
        type_prompt();
        read_command();
        if (fork() != 0) {
            waitpid(-1, &status, 0);
        } else {
            char *real_command = get_location(command);
            execve(real_command, parameters, 0);
        }
    } 
}