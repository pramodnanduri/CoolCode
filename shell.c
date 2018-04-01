#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define LINE_BUFFERSIZE 64
#define LINESIZE 255
#define DELIMITER " \t\r\n\a"

int executeCommand(char **args) {
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid == 0) { // Child
        if (execvp(args[0], args) == -1) {
            perror("myshell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("myshell");
    } else {
        printf("myshell: started child pid %d \n", pid);
        waitpid(pid, &status, 0);
        
    }
    return 1;
}

int parseCommand(char **args, int count) {
    if (args == NULL) {
        return 1;
    }
    if (strcmp(*args, "exit") == 0) {
        return 0;
    } else if (strcmp(*args, "run") == 0) {
        char *command[20] = {};
        int k = 1;
        int j = 0;
        for (int i =1; i < count; i++) {
            if(strcmp(args[i], ";") == 0) {
                k = 1;
                executeCommand(command);
                j = 0;
                memset(command, 0, sizeof(command));
            } else {
                k = 0;
                command[j] = args[i];
                
                j++;
            }
        }
        if (k == 0) {
            executeCommand(command);
            memset(command, 0, sizeof(command));
        }
        return 1;
    }
    printf("myshell: %s is not a valid command \n", args[0]);
    return 1;
}


int execute(void) {
    char *line = malloc(sizeof(char) * BUFFER_SIZE);
    fgets(line, LINESIZE, stdin);
    if (strlen(line) == 0) {
        return 1;
    }
    
    int i = 0;
    char **words = (char **) malloc(BUFFER_SIZE * sizeof(char*));
    words[i] = strtok(line, " \n");
    while (words[i] != NULL) {
        //printf("words[%d] is %s\n", i, words[i]);
        i++;
        words[i] = strtok(NULL, " \n");
    }
    words[i + 1] = NULL;
    return parseCommand(words, i );
}

int main1(int argc, char **argv) {
    int status;
    
    do {
        printf("myshell> ");
        status = execute();
    } while (status);
    return EXIT_SUCCESS;
}

