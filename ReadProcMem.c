#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// This program reads the information from /proc/meminfo and prints it to the terminal.

int ReadProcMem(unsigned long long *totMem, unsigned long long *memAvail) 
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
            sscanf(line, "MemTotal: %llu kB", totMem); // Read total memory from the line
            sscanf(line, "MemFree: %llu kB", memAvail); // Read free
        }
    }
    fclose(file);
    return 0; //return to indicate success
}
