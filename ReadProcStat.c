#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// This program reads the information from /proc/stat and prints it to the terminal.

typedef struct CPUData {
    int user; 
    int nice;
    int system;
    int idle;
    int iowait;
    int irq;
    int softirq;
    int steal;
    int guest;
    int guest_nice;

}cpuData; // Each field in the line read: user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice



int main() 
{
    cpuData cpu;

    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL) 
    {
        printf("Failed to open stat info");
        return 0; //return to indicate failure
    }

    else
    {
    char line[500];
        fgets(line, sizeof(line), file); // For now we'll read first line since it contains the overall cpu core usage
        //while (fgets(line, sizeof(line), file)) {} (We'll use it if we want to read each line for each cpu core usage)

        printf("%s", line); // Print line of the stat info
        
        // Since each field is raw data, we need to parse to determine CPU Usage. (Idle = idle + iowait) (nonIdle = user + nice + system + irq + softirq + steal) (total = idle + nonIdle)
        char *token = strtok(line, " "); 
        for(int i = 0; token != NULL; i++) 
        {
            if (i == 1) 
            {
                cpu.user = atoi(token);
            }
            if (i == 2) 
            {
                cpu.nice = atoi(token);
            }
            if (i == 3) 
            {
                cpu.system = atoi(token);
            }
            if (i == 4) 
            {
                cpu.idle = atoi(token);
            }
            if (i == 5) 
            {
                cpu.iowait = atoi(token);
            }
            if (i == 6) 
            {
                cpu.irq = atoi(token);
            }
            if (i == 7) 
            {
                cpu.softirq = atoi(token);
            }
            if (i == 8) 
            {
                cpu.steal = atoi(token);
            }
            if (i == 9) 
            {
                cpu.guest = atoi(token);
            }
            if (i == 10) 
            {
                cpu.guest_nice = atoi(token);
            }

            token = strtok(NULL, " ");
        }
        int idle = cpu.idle + cpu.iowait;
        int nonIdle = cpu.user + cpu.nice + cpu.system + cpu.irq + cpu.softirq + cpu.steal;
        int total = idle + nonIdle;
        double Percentage = ((double)nonIdle / total) * 100;
        printf("CPU Usage: %.2f%%\n", Percentage);
    }
    fclose(file);
    return 1; //return to indicate success
}