#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 80
#define MAX_COMMANDS 4
#define TOKENS 8

void setting_up_pipes(char commands[MAX_COMMANDS][MAX_COMMAND_LENGTH], int num_commands){
   int fd[num_commands - 1][2];
   for (int i = 0; i< num_commands -1; i++){
        pipe(fd[i]);
    }
    int var = 0;
    while(var < num_commands) {
        char *cmd[TOKENS];
        int count = 0;
        char *token = strtok(commands[var], " ");
        
        while(token!= NULL) {
            cmd[count] = token;
            token = strtok(NULL, " ");
            count ++;
            
        
        }
        cmd[count] = NULL;  

        int pid = fork();
        if(pid ==0){
            if(var != 0){
                dup2(fd[var - 1][0], 0);
            }
            if(var < num_commands - 1){
                dup2(fd[var][1], 1);
            }

            for(int i = 0; i < num_commands - 1; i++){
                close(fd[i][0]);
                close(fd[i][1]);
            }
            execve(cmd[0], cmd, NULL);
            exit(1);


    }
    else{
        if(var!=0) {
                close(fd[var-1][0]);
                close(fd[var-1][1]);
            }
        }
        waitpid(-1, NULL, 0);
        var++;
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