#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// This program reads the information from /proc/stat and prints it to the terminal.

typedef struct CPUData {
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
    unsigned long long guest;
    unsigned long long guest_nice;

}cpuData; // Each field in the line read: user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice

void initCPUData(cpuData *CPU, FILE *file); 

int main() 
{
    cpuData cpu;
    cpuData cpuNext; 

    FILE *file = fopen("/proc/stat", "r");

    if (file == NULL) 
    {
        printf("Failed to open stat info");
        return 0; //return to indicate failure
    }
        
    initCPUData(&cpu, file); 
    fclose(file);    

    int idle0 = cpu.idle + cpu.iowait;
    int nonIdle0 = cpu.user + cpu.nice + cpu.system + cpu.irq + cpu.softirq + cpu.steal;
    int total0 = idle0 + nonIdle0;

    //Delay to read the next line of the stat info to get the next cpu usage data
    sleep(1);
    FILE *fileNext = fopen("/proc/stat", "r");
    if (fileNext == NULL) 
    {
        printf("Failed to open stat info");
        return 1; //return to indicate failure
    }
        
    initCPUData(&cpuNext, fileNext); 
    fclose(fileNext);    
        
    int idle1 = cpuNext.idle + cpuNext.iowait;
    int nonIdle1 = cpuNext.user + cpuNext.nice + cpuNext.system + cpuNext.irq + cpuNext.softirq + cpuNext.steal;
    int total1 = idle1 + nonIdle1;

    int totald = total1 - total0;
    int idled = idle1 - idle0;

    double Percentage = (double)(totald - idled) / totald * 100;

    printf("CPU Usage: %.2f%%\n", Percentage);
    
    return 0; //return to indicate success
}

void initCPUData(cpuData *CPU, FILE *file)
{
    char line[500];
    fgets(line, sizeof(line), file); // For now we'll read first line since it contains the overall cpu core usage
    //while (fgets(line, sizeof(line), file)) {} (We'll use it if we want to read each line for each cpu core usage)

    printf("Initial Run: %s", line); // Print line of the stat info
        
    // Since each field is raw data, we need to parse to determine CPU Usage. (Idle = idle + iowait) (nonIdle = user + nice + system + irq + softirq + steal) (total = idle + nonIdle)
    char *token = strtok(line, " "); 
    for(int i = 0; token != NULL; i++) 
    {
        if (i == 1) 
        {
            CPU->user = strtoull(token, NULL, 10); // Convert string to unsigned long long. Prevent overflow since the number of bytes can be very large.
        }
        if (i == 2) 
        {
            CPU->nice = strtoull(token, NULL, 10);
        }
        if (i == 3) 
        {
            CPU->system = strtoull(token, NULL, 10);
        }
        if (i == 4) 
        {
            CPU->idle = strtoull(token, NULL, 10);
        }
        if (i == 5) 
        {
            CPU->iowait = strtoull(token, NULL, 10);
        }
        if (i == 6) 
        {
            CPU->irq = strtoull(token, NULL, 10);
        }
        if (i == 7) 
        {
            CPU->softirq = strtoull(token, NULL, 10);
        }
        if (i == 8) 
        {
            CPU->steal = strtoull(token, NULL, 10);
        }
        if (i == 9) 
        {
            CPU->guest = strtoull(token, NULL, 10);
        }
        if (i == 10) 
        {
            CPU->guest_nice = strtoull(token, NULL, 10);
        }

        token = strtok(NULL, " ");
    }

}