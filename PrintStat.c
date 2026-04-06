#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ReadProcStat.c"
#include "ReadProcMem.c"
#include "ReadProcNet.c"

int main() 
{
    while(1)
    {
        double cpu; 
        unsigned long long totMem, memFree;
        Receive receive; Transmit transmit; 
        cpu = ReadProcStat();
        ReadProcMem(&totMem, &memFree);
        ReadProcNet(&receive, &transmit);

        printf("{ \"cpu\": %.2f, \"mem_total\": %llu, \"mem_available\": %llu, \"net_recv\": %llu, \"net_sent\": %llu }\n",
                cpu, totMem, memFree, receive.bytes, transmit.bytes);

        sleep(2);
    }

    return 0; //return to indicate success
}