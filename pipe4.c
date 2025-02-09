#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 80
#define MAX_COMMANDS 4
#define TOKENS 8

void setting_up_pipes(char commands[MAX_COMMANDS][MAX_COMMAND_LENGTH], int num_commands){
   int fd[num_commands - 1][2];
   pid_t pids[MAX_COMMANDS];

   for(int i = 0; i < num_commands - 1; i++){
      if(pipe(fd[i]) == -1){
        exit(1);
      }
   }
   for(int i = 0; i< num_commands; i++){
        pids[i] = fork();
        if(pids[i] == -1){
            exit(1);
        }
   
        if(pids[i] == 0){
            if (i > 0) {
                if (dup2(fd[i - 1][0], STDIN_FILENO) == -1) {
                    exit(1);
                }
        }
        if (i < num_commands - 1) {
                if (dup2(fd[i][1], STDOUT_FILENO) == -1) {
                    exit(1);
                }
            }
            for (int j = 0; j < num_commands - 1; j++) {
                close(fd[j][0]);
                close(fd[j][1]);
            }

            // Tokenize the command
            char *args[TOKENS];
            char *token = strtok(commands[i], " \n");
            int k = 0;
            while (token != NULL && k < TOKENS - 1) {
                args[k++] = token;
                token = strtok(NULL, " \n");
            }
            args[k] = NULL;

            // Execute the command
            execvp(args[0], args); 
            exit(1);
        }
    }

    // Close all pipe file descriptors in the parent
    for (int i = 0; i < num_commands - 1; i++) {
        close(fd[i][0]);
        close(fd[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }




 }
int main() {
    char command[MAX_COMMAND_LENGTH] = "";
    char commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];
    int num_commands = 0;
    int count = 1;

    while (count <= MAX_COMMANDS)
    {
        
        if (read(0, command, MAX_COMMAND_LENGTH) > 1)
        {
            command[strlen(command) - 1] = '\0';
            strcpy(commands[num_commands], command);
            num_commands++;
            
        }
        else
        {
            break;
        }
        count++;
    }

    setting_up_pipes(commands, num_commands);

    return 0;
}