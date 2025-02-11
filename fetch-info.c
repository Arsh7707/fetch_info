#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 5000


// printing the cpu model name:
void printing_cpu_model(){
    FILE *file = fopen("/proc/cpuinfo", "r"); // file open pointer from tutorial followed
    if(!file){
        printf("Error opening file");
        return;
    }

    char string[BUFFER_SIZE];
    while(fgets(string, BUFFER_SIZE, file)){ // reading the file line by line
        if(strncmp(string, "model name:",10) == 0){ // comparing the string with model name
            char *model_name = strchr(string, ':'); // finding the first occurence of the character ':' found this on stackoverflow
            if (model_name) {
                printf("%-13s %s", "model name:", model_name+=2);  // printing the model name, and %-13s has been used for spacing, helped by AI tool for spacing issues, also I could have added "" in between but wanted to try something new
            }
           break;
        }
    }
    fclose(file); // closing the file
}   
// printing the cpu cores:
void printing_cpu_cores(){
    FILE *file = fopen("/proc/cpuinfo", "r"); // same logic as above
    if(!file){
        printf("Error opening file");
        return;
    }
    char line[BUFFER_SIZE];
    while(fgets(line, BUFFER_SIZE, file)){
        if(strncmp(line, "cpu cores:",9) == 0){ // same logic as above
            char *value = strchr(line, ':');
            if (value) {
                printf("%-13s %s", "cpu cores:", value+=2); // same logic as above
            }
            break;
        }
    }
    fclose(file);

}
// printing the linux version: and all same logic as above nothing new
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
// printing the total memory:
void printing_total_memory() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char label[50];
    unsigned long mem_kb; // unsigned long used for memory

    while (fscanf(file, "%s %lu kB", label, &mem_kb) == 2) {
        if (strcmp(label, "MemTotal:") == 0) { // comparing the label with MemTotal
            printf("%-11s %lu kB\n", "MemTotal:", mem_kb);
            break;
        }
    }
    fclose(file);
}
//printing the uptime:
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

    int days = (int)(uptime_seconds / 86400); // converting the seconds to days, hours, minutes and seconds, used google for the formula
    int hours = (int)((uptime_seconds - (days * 86400)) / 3600); // converting the seconds to days, hours, minutes and seconds
    int minutes = (int)((uptime_seconds - (days * 86400) - (hours * 3600)) / 60); // converting the seconds to days, hours, minutes and seconds
    int seconds = (int)(uptime_seconds) % 60; // converting the seconds to days, hours, minutes and seconds

    printf("Uptime: %d days, %d hours, %d minutes, %d seconds\n", days, hours, minutes, seconds);
}
// printing the process number: and same logic as above
void printing_process_number(int pid){
    char string[BUFFER_SIZE];
    snprintf(string, sizeof(string), "/proc/%d/comm", pid);
    FILE *file = fopen(string, "r");
    if(!file){
        printf("Process number %d not found\n", pid);
        exit(1);
    }
    printf("Process number:   %d\n", pid);
    fclose(file);
}
// printing the process name: and same logic as above just pasing an argument pid it will work with ./fetch-info <pid>
void printing_process_name(int pid){
    char string[BUFFER_SIZE];
    snprintf(string, sizeof(string), "/proc/%d/comm", pid); // using snprintf to store the pid in string and the process name can be found in "/proc/%d/comm"
    FILE *file = fopen(string, "r");
    if(!file){
        printf("Error opening file");
        exit(1);
    }
    char process_name[BUFFER_SIZE];
    if(fgets(process_name, sizeof(process_name), file)){; // reading the file line by line and storing it in process_name
        printf("Name:   %s", process_name); // here I decided to put spaces normally
    } else {
        printf("Name:   [Unable to read process name]\n");
    }
    fclose(file);    

    
}
// printing the file name: and same logic as above
void printing_file_name(int pid){
    char string[BUFFER_SIZE];
    snprintf(string, sizeof(string), "/proc/%d/cmdline", pid);
    FILE *file = fopen(string, "r");
    if(!file){
        printf("Error opening file");
        exit(1);
    }
    char file_name[BUFFER_SIZE];
    if(fgets(file_name, sizeof(file_name), file)){
        printf("Filename (if any):   %s\n", file_name);
    } else {
        printf("Filename (if any):   may be blank\n");
    }
    fclose(file);
}
// printing the number of threads: and same logic as above
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
    int count = 0; // need a count for threads at which line my function will work, primarily I used for debugging
    while(fgets(line, BUFFER_SIZE, file)){
        if(strncmp(line, "Threads:",8) == 0){ // comparing the string with Threads counting the characters basically
            count+=1;
            sscanf(line, "Threads: %d", &threads); // scanning the line for threads
            printf("Threads:    %d\n", threads);
            break;
            
        }
        
    }
    if(count == 0){
        printf("Threads:    [Unable to read number of threads]\n");
    }
    fclose(file);        
    
}
// printing the context switches: and same logic as above
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
        if(strncmp(line, "voluntary_ctxt_switches:",24) == 0){ // comparing the string with voluntary_ctxt_switches
            sscanf(line, "voluntary_ctxt_switches: %d", &voluntary_ctxt_switches);
        }
        if(strncmp(line, "nonvoluntary_ctxt_switches:",27) == 0){
            sscanf(line, "nonvoluntary_ctxt_switches: %d", &nonvoluntary_ctxt_switches);
        }
    }
    printf("Total context switches:    %d\n", voluntary_ctxt_switches + nonvoluntary_ctxt_switches);
    fclose(file);

} // main function and calling out the above functions
int main(int argc, char *argv[]) {
    if (argc == 1) {
        printing_cpu_model();
        printing_cpu_cores();
        printing_linux_version();
        printing_total_memory();
        printing_uptime();
    }
    else if(argc == 2){ // checking for how many arguments are passed
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
