#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void cpu_model(){
    FILE *file = fopen("/proc/cpuinfo", "r");
    if(!file){
        printf("Error: Unable to open file\n");
        return;
    }

    char line[BUFFER_SIZE];
    while(fgets(line, BUFFER_SIZE, file)){
        if(strncmp(line, "model name:",10) == 0){
            char *model_name = strchr(line, ':');
            if (model_name) {
                printf("model name: %s", model_name + 2);  // Skip ': 
            }
           break;
        }
    }
    fclose(file);
}   

void cpu_cores(){
    FILE *file = fopen("/proc/cpuinfo", "r");
    if(!file){
        printf("Error: Unable to open file\n");
        return;
    }
    int cores = 0;
    char line[BUFFER_SIZE];
    while(fgets(line, BUFFER_SIZE, file)){
        if(strncmp(line, "cpu cores:",9) == 0){
            char *value = strchr(line, ':');
            if (value) {
                printf("cpu cores: %s", value + 2);  // Skip ": "
            }
            break;  // Stop after finding the first occurrence
        }
    }
    fclose(file);
    printf("cpu cores: %d\n", cores);

}

void linux_version() {
    FILE *file = fopen("/proc/version", "r");
    if (!file) {
        perror("Error opening /proc/version");
        return;
    }

    char version[BUFFER_SIZE];
    fgets(version, sizeof(version), file);
    fclose(file);

    printf("%s", version);
}

void total_memory() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        perror("Error opening /proc/meminfo");
        return;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemTotal", 8) == 0) {
            printf("%s", line);
            break;
        }
    }
    fclose(file);
}
void uptime() {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) {
        perror("Error opening /proc/uptime");
        return;
    }

    double uptime_seconds;
    fscanf(file, "%lf", &uptime_seconds);
    fclose(file);

    int days = (int)(uptime_seconds / 86400);
    int hours = (int)((uptime_seconds - (days * 86400)) / 3600);
    int minutes = (int)((uptime_seconds - (days * 86400) - (hours * 3600)) / 60);
    int seconds = (int)(uptime_seconds) % 60;

    printf("Uptime: %d days, %d hours, %d minutes, %d seconds\n", days, hours, minutes, seconds);
}
int main(int argc, char *argv[]) {
    // If the program is run without arguments, retrieve and print system information
    if (argc == 1) {
        cpu_model();
        cpu_cores();
        linux_version();
        total_memory();
        uptime();
    } 
    // else {
    //     printf("Invalid usage: This program does not accept arguments (yet).\n");
    // }

    return 0;
}
