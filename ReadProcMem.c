#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// This program reads the information from /proc/meminfo and prints it to the terminal.

int ReadProcMem(unsigned long long *totMem, unsigned long long *memFree) 
{
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        printf("Failed to open memory info");
        return 1; //return to indicate failure
    }

    else
    {
    char line[500];
        while (fgets(line, sizeof(line), file)) 
        {
            //printf("%s", line); // Print each line of the memory info
            sscanf(line, "MemTotal: %llu kB", totMem); // Read total memory from the line
            sscanf(line, "MemFree: %llu kB", memFree); // Read free
        }
    }
    fclose(file);
    return 0; //return to indicate success
}