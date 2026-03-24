#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// This program reads the network interface information from /proc/net/dev and prints it to the terminal.

typedef struct ReceivedData {
    char networkName[20]; 
    int bytes;
    int packets;
    int errs;
    int drop;
    int fifo;
    int frame;
    int compressed;
    int multicast;

}Receive; 

typedef struct TransmittedData {
    char networkName[20]; 
    int bytes;
    int packets;
    int errs;
    int drop;
    int fifo;
    int colls;
    int carrier;
    int compressed;
}Transmit;



int main() 
{
    Transmit transmit; 
    Receive receive; 

    FILE *file = fopen("/proc/net/dev", "r");
    if (file == NULL) 
    {
        printf("Failed to open network interface info");
        return 0; //return to indicate failure
    }

    else
    {
    char buffer[500];
    char line[500];
        int i = 0; 
        while(fgets(buffer, sizeof(buffer), file)) 
        {
            if (i == 2) 
            {
                break; // Stop reading after the second line (header lines)  & lo (loopback interface) since we want to read the ethernet interface (internet traffic)
            }
            i++;
        }

        fgets(line, sizeof(line), file); // Read line of the network interface info
        printf("%s", line); 
        
        char *token = strtok(line, " "); 

        //Recieve 
        for(int i = 0; token != NULL; i++) 
        {
            if (i == 0) 
            {
                strcpy(receive.networkName, token);
            }
            if (i == 1) 
            {
                receive.bytes = atoi(token);
            }
            if (i == 2) 
            {   
                receive.packets = atoi(token);
            }
            if (i == 3) 
            {
                receive.errs = atoi(token);
            }
            if (i == 4) 
            {
                receive.drop = atoi(token);
            }
            if (i == 5) 
            {
                receive.fifo = atoi(token);
            }
            if (i == 6) 
            {
                receive.frame = atoi(token);
            }
            if (i == 7) 
            {
                receive.compressed = atoi(token);
            }
            if (i == 8) 
            {
                receive.multicast = atoi(token);
            }   
            token = strtok(NULL, " ");
        }

        //Transmit
         strcpy(transmit.networkName, receive.networkName);
        for(int j = 0; token != NULL; j++)
        {
            if (j == 0) 
            {
               transmit.bytes = atoi(token);
            }
            if (j == 1) 
            {
                transmit.packets = atoi(token);
            }
            if (j == 2) 
            {   
                transmit.errs = atoi(token);
            }
            if (j == 3) 
            {   
                transmit.drop = atoi(token);
            }
            if (j == 4) 
            {
                transmit.fifo = atoi(token);
            }
            if (j == 5) 
            {
                transmit.colls = atoi(token);
            }
            if (j == 6) 
            {
                transmit.carrier = atoi(token);
            }
            if (j == 7) 
            {
                transmit.compressed = atoi(token);
            }
            token = strtok(NULL, " ");
        }

        printf("Network Interface: %s\n", receive.networkName);
        printf("Received: %d bytes,%d drop\n", receive.bytes, receive.drop); 
        printf("Transmitted: %d bytes, %d drop\n", transmit.bytes, transmit.drop);
    }
    fclose(file);
    return 1; //return to indicate success
}