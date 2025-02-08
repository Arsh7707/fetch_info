#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 5000

void printing_cpu_model(){
    FILE *file = fopen("/proc/cpuinfo", "r");
    if(!file){
        printf("Error opening file");
        return;
    }

    char string[BUFFER_SIZE];
    while(fgets(string, BUFFER_SIZE, file)){
        if(strncmp(string, "model name:",10) == 0){
            char *model_name = strchr(string, ':');
            if (model_name) {
                printf("%-13s %s", "model name:", model_name+=2); 
            }
           break;
        }
    }
    fclose(file);
}   

void printing_cpu_cores(){
    FILE *file = fopen("/proc/cpuinfo", "r");
    if(!file){
        printf("Error opening file");
        return;
    }
    char line[BUFFER_SIZE];
    while(fgets(line, BUFFER_SIZE, file)){
        if(strncmp(line, "cpu cores:",9) == 0){
            char *value = strchr(line, ':');
            if (value) {
                printf("%-13s %s", "cpu cores:", value+=2);
            }
            break;
        }
    }
    fclose(file);

}

void printing_linux_version() {
    FILE *file = fopen("/proc/version", "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char version[BUFFER_SIZE];
    if (fgets(version, sizeof(version), file) != NULL) {
        printf("%s", version);
    } else {
        printf("[Unable to read version]\n");
    }
    fclose(file);
}

void printing_total_memory() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char label[50];
    unsigned long mem_kb;

    while (fscanf(file, "%s %lu kB", label, &mem_kb) == 2) {
        if (strcmp(label, "MemTotal:") == 0) {
            printf("%-11s %lu kB\n", "MemTotal:", mem_kb);
            break;
        }
    }
    fclose(file);
}
void printing_uptime() {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    double uptime_seconds;
    if (fscanf(file, "%lf", &uptime_seconds) != 1) {
        printf("[Unable to read uptime]\n");
        fclose(file);
        return;
    }
    fclose(file);

    int days = (int)(uptime_seconds / 86400);
    int hours = (int)((uptime_seconds - (days * 86400)) / 3600);
    int minutes = (int)((uptime_seconds - (days * 86400) - (hours * 3600)) / 60);
    int seconds = (int)(uptime_seconds) % 60;

    printf("Uptime: %d days, %d hours, %d minutes, %d seconds\n", days, hours, minutes, seconds);
}

void printing_process_number(int pid){
    char string[BUFFER_SIZE];
    snprintf(string, sizeof(string), "/proc/%d/comm", pid);
    FILE *file = fopen(string, "r");
    if(!file){
        printf("Process number: %d not found\n", pid);
        exit(1);
    }
    printf("Process number:   %d\n", pid);
    fclose(file);
}

void printing_process_name(int pid){
    char string[BUFFER_SIZE];
    snprintf(string, sizeof(string), "/proc/%d/comm", pid);
    FILE *file = fopen(string, "r");
    if(!file){
        printf("Error opening file");
        exit(1);
    }
    char process_name[BUFFER_SIZE];
    if(fgets(process_name, sizeof(process_name), file)){;
        printf("Name:   %s", process_name);
    } else {
        printf("Name:   [Unable to read process name]\n");
    }
    fclose(file);    

    
}
void printing_file_name(int pid){
    char string[BUFFER_SIZE];
    snprintf(string, sizeof(string), "/proc/%d/cmdline", pid);
    FILE *file = fopen(string, "rb");
    if(!file){
        printf("Error opening file");
        exit(1);
    }
    char file_name[BUFFER_SIZE];
    size_t len = fread(file_name, 1, sizeof(file_name) - 1, file);
    fclose(file);

    if (len > 0) {
        file_name[len] = '\0';  // Null-terminate the string

        // Replace all null characters with spaces for proper display
        printf("Filename (if any):   ");
        for (size_t i = 0; i < len; i++) {
            if (file_name[i] == '\0') {
                printf(" ");
            } else {
                printf("%c", file_name[i]);
            }
        }
        printf("\n");
    } else {
        printf("Filename (if any):   may be blank\n");
    }
}
void counting_threads(int pid){
    char string[BUFFER_SIZE];
    snprintf(string, sizeof(string), "/proc/%d/status", pid);
    FILE *file = fopen(string, "r");
    if(!file){
        printf("Error opening file");
        exit(1);
    }
    int threads = 0;
    char line[BUFFER_SIZE];
    int count = 0;
    while(fgets(line, BUFFER_SIZE, file)){
        if(strncmp(line, "Threads:",8) == 0){
            count+=1;
            sscanf(line, "Threads: %d", &threads);
            printf("Threads:    %d\n", threads);
            break;
            
        }
        
    }
    if(count == 0){
        printf("Threads:    [Unable to read number of threads]\n");
    }
    fclose(file);        
    
}
void printing_context_switches(int pid){
    char string[BUFFER_SIZE];
    snprintf(string, sizeof(string), "/proc/%d/status", pid);
    FILE *file = fopen(string, "r");
    if(!file){
        printf("Error opening file");
        exit(1);
    }
    int voluntary_ctxt_switches = 0;
    int nonvoluntary_ctxt_switches = 0;
    char line[BUFFER_SIZE];
    while(fgets(line, BUFFER_SIZE, file)){
        if(strncmp(line, "voluntary_ctxt_switches:",24) == 0){
            sscanf(line, "voluntary_ctxt_switches: %d", &voluntary_ctxt_switches);
        }
        if(strncmp(line, "nonvoluntary_ctxt_switches:",27) == 0){
            sscanf(line, "nonvoluntary_ctxt_switches: %d", &nonvoluntary_ctxt_switches);
        }
    }
    printf("Total context switches:    %d\n", voluntary_ctxt_switches + nonvoluntary_ctxt_switches);
    fclose(file);
}
int main(int argc, char *argv[]) {
    if (argc == 1) {
        printing_cpu_model();
        printing_cpu_cores();
        printing_linux_version();
        printing_total_memory();
        printing_uptime();
    }
    else if(argc == 2){
        int pid = atoi(argv[1]);
        if(pid <= 0){
            printf("Process Number %d not found\n", pid);
            exit(1);
        }
        printing_process_number(pid);
        printing_process_name(pid);
        printing_file_name(pid);
        counting_threads(pid);
        printing_context_switches(pid);
    }
    return 0;
}
