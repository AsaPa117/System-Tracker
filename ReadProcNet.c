#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// This program reads the network interface information from /proc/net/dev and prints it to the terminal.

typedef struct ReceivedData {
    char networkName[20]; 
    unsigned long long bytes;
    unsigned long long packets;
    unsigned long long errs;
    unsigned long long drop;
    unsigned long long fifo;
    unsigned long long frame;
    unsigned long long compressed;
    unsigned long long multicast;

}Receive; 

typedef struct TransmittedData {
    char networkName[20]; 
    unsigned long long bytes;
    unsigned long long packets;
    unsigned long long errs;
    unsigned long long drop;
    unsigned long long fifo;
    unsigned long long colls;
    unsigned long long carrier;
    unsigned long long compressed;
}Transmit;

void initNetworkData(Transmit *TRANSMIT, Receive *RECEIVE, char line[500]); 


int main() 
{
    Transmit transmit; 
    Receive receive; 

    FILE *file = fopen("/proc/net/dev", "r");
    if (file == NULL) 
    {
        printf("Failed to open network interface info");
        return 1; //return to indicate failure
    }

   
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

        //Recieve & Transmit 
        initNetworkData(&transmit, &receive, line); 

        printf("Network Interface: %s\n", receive.networkName);
        printf("Received: %llu bytes, %llu drop\n", receive.bytes, receive.drop); 
        printf("Transmitted: %llu bytes, %llu drop\n", transmit.bytes, transmit.drop);
    
    fclose(file);
    return 0; //return to indicate success
}

void initNetworkData(Transmit *TRANSMIT, Receive *RECEIVE, char line[500]) 
{
    char *token = strtok(line, " :\t\n"); 

    for(int j = 0; token != NULL; j++)
        {
            if (j == 0) 
            {
               strcpy(RECEIVE->networkName, token);
               strcpy(TRANSMIT->networkName, token);
            }
            if (j == 1) 
            {
                RECEIVE->bytes = strtoull(token, NULL, 10); // Convert string to unsigned long long. Prevent overflow since the number of bytes can be very large.
            }
            if (j == 2) 
            {   
                RECEIVE->packets = strtoull(token, NULL, 10);
            }
            if (j == 3) 
            {   
                RECEIVE->errs = strtoull(token, NULL, 10);
            }
            if (j == 4) 
            {
                RECEIVE->drop = strtoull(token, NULL, 10);
            }
            if (j == 5) 
            {
                RECEIVE->fifo = strtoull(token, NULL, 10);
            }
            if (j == 6) 
            {
                RECEIVE->frame = strtoull(token, NULL, 10);
            }
            if (j == 7) 
            {
                RECEIVE->compressed = strtoull(token, NULL, 10);
            }
            if (j == 8) 
            {
                RECEIVE->multicast = strtoull(token, NULL, 10);
            }
            if (j == 9) 
            {
                TRANSMIT->bytes = strtoull(token, NULL, 10);
            }
            if (j == 10) 
            {TRANSMIT->packets = strtoull(token, NULL, 10);
            }
            if (j == 11) 
            {TRANSMIT->errs = strtoull(token, NULL, 10);
            }
            if (j == 12) 
            {TRANSMIT->drop = strtoull(token, NULL, 10);
            }
            if (j == 13) 
            {TRANSMIT->fifo = strtoull(token, NULL, 10);
            }
            if (j == 14) 
            {TRANSMIT->colls = strtoull(token, NULL, 10);
            }
            if (j == 15) 
            {TRANSMIT->carrier = strtoull(token, NULL, 10);
            }
            if (j == 16) 
            {TRANSMIT->compressed = strtoull(token, NULL, 10);
            }

            token = strtok(NULL, " :\t\n");
        
        }
}
