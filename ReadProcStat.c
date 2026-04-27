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

double ReadProcStat() 
{
    static unsigned long long prev_idle = 0, prev_total = 0; 

    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL) 
    {
        printf("Failed to open stat info");
        return 0; //return to indicate failure
    } 

    FILE *fileNext = fopen("/proc/stat", "r");
    if (fileNext == NULL) 
    {
        printf("Failed to open stat info");
        return 1; //return to indicate failure
    }

     cpuData cpu;
    initCPUData(&cpu, file); 
    fclose(file);   

    unsigned long long idle = cpu.idle + cpu.iowait; 
    unsigned long long total = idle + cpu.user + cpu.nice + cpu.system + cpu.irq + cpu.softirq + cpu.steal; 
    
    if(prev_total == 0)
    {
    prev_total = total; 
    prev_idle = idle; 
    return 0.0; 
    } 
    
    unsigned long long totald = total - prev_total; 
    unsigned long long idled = idle - prev_idle; 

    prev_total = total; 
    prev_idle = idle; 

    if(totald == 0) {
        return 0.0; 
    }

    double Percentage = (double)(totald - idled) / totald * 100;
    return Percentage; //return to indicate success
}

void initCPUData(cpuData *CPU, FILE *file)
{
    char line[500];
    fgets(line, sizeof(line), file); // For now we'll read first line since it contains the overall cpu core usage
        
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
