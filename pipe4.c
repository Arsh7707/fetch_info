#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 80 // defining the max command length
#define MAX_COMMANDS 4 //   defining the max commands
#define TOKENS 8 // defining the size of tokens

// setting up the pipes and main logic
void setting_up_pipes(char commands[MAX_COMMANDS][MAX_COMMAND_LENGTH], int num_commands){
   int fd[num_commands - 1][2]; // Array to hold file descriptors for pipes

   // Create pipes
   for (int i = 0; i< num_commands -1; i++){ 
        if (pipe(fd[i]) == -1) {
            exit(1);
        }
    }
    int var = 0;
    while(var < num_commands) {
        char *cmd[TOKENS]; // Array to hold tokens of the current command
        int count = 0;
        char *token = strtok(commands[var], " "); // Tokenize the command
        
        // Split the command into tokens
        while(token!= NULL) {
            cmd[count] = token;
            token = strtok(NULL, " ");
            count ++;
            
        
        }
        cmd[count] = NULL;   // Null-terminate the array of tokens

        int pid = fork(); // Fork a new process
        if(pid ==0){ // Child process
        // If not the first command, redirect input from the previous pipe
            if(var != 0){
                dup2(fd[var - 1][0], 0);
            }
            // If not the last command, redirect output to the next pipe
            if(var < num_commands - 1){
                dup2(fd[var][1], 1);
            }
             // Close all pipe file descriptors in the child process

            for(int i = 0; i < num_commands - 1; i++){
                close(fd[i][0]);
                close(fd[i][1]);
            }
            execve(cmd[0], cmd, NULL); // Execute the command
            exit(1);


    }
    else{
         // Close the read and write ends of the previous pipe
        if(var!=0) {
                close(fd[var-1][0]);
                close(fd[var-1][1]);
            }
        }
        // Wait for the child process to finish
        waitpid(-1, NULL, 0);
        var++;
    }




 }
int main() {
    char command[MAX_COMMAND_LENGTH] = "";
    char commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];
    int count = 1;
    int num_commands = 0;
  
    // Read commands from standard input
    while (count <= MAX_COMMANDS)
    {
        
        if (read(0, command, MAX_COMMAND_LENGTH) > 1)
        {
            command[strlen(command) - 1] = '\0';
            strcpy(commands[num_commands], command); // Copy the command to the array
            num_commands++;
            
        }
        else
        {
            break;
        }
        count++;
    }
    // Set up pipes and execute commands
    setting_up_pipes(commands, num_commands);

    return 0;
}