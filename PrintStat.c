#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ReadProcStat.c"
#include "ReadProcMem.c"
#include "ReadProcNet.c"
//Socket Server
#include <sys/socket.h>  // socket, bind, listen, accept, send
#include <netinet/in.h> // sockaddr_in, INADDR_ANY
#include <unistd.h> // close()
#include <arpa/inet.h> // htons (converts port number to network byte order)

#include <sys/select.h> // select() for handling multiple clients
#include <time.h> // time() for tracking when to send data to clients
 
time_t prev = 0;

fd_set stats_clients; 

int main() 
{    
    //Base on GeeksForGeeks  
    // SETUP (runs once)
    // 1. create socket file descriptor
    int dev_server = socket(AF_INET, SOCK_STREAM, 0); // AF_INET for IPv4, SOCK_STREAM for TCP
    if (dev_server == -1) {
        printf("Failed to create socket");
        return 1; //return to indicate failure
    }
 
    // 2. bind to port 5555
    struct sockaddr_in address; // Structure to hold server address information
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
    address.sin_port = htons(5555); // Convert port number to network byte order
    if (bind(dev_server, (struct sockaddr*)&address, sizeof(address)) < 0) 
    {
        printf("Bind failed");
        close(dev_server); 
        return 1;
    }
 
    // 3. start listening
    if (listen(dev_server, 3) < 0) 
    {
        printf("Listen Failed");
        close(dev_server);
        return 1;
    }
 
    printf("Server running on port 5555\n"); 
 
    //update: Handle mutiple clients
    fd_set master;
    fd_set read_fds;
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_ZERO(&stats_clients); 
    FD_SET(dev_server, &master);
    int fdmax = dev_server;


    time_t cur; 
    // (update) LOOP to accept multiple connections (one for HTML page, one for stats data)
    while(1)
    {
        // update: Send data to clients every 2 seconds
        cur = time(NULL);
        if(cur - prev >= 2)
        {
            prev = cur;
            // READ/WRITE (runs repeatedly until client disconnects)
            
            char buffer[1024]; // Buffer to hold data to send to client
            double cpu; 
            unsigned long long totMem, memFree;
            Receive receive; Transmit transmit; 
            cpu = ReadProcStat();
            ReadProcMem(&totMem, &memFree);
            ReadProcNet(&receive, &transmit);
            sprintf(buffer, "{ \"cpu\": %.2f, \"mem_total\": %llu, \"mem_available\": %llu, \"net_recv\": %llu, \"net_sent\": %llu }\n",
                    cpu, totMem, memFree, receive.bytes, transmit.bytes); //store data in buffer as a string
 
            int k = 0;
            while(k <= fdmax)
            {
                if (FD_ISSET(k, &stats_clients))  
                {   
                    
                    if (send(k, buffer, strlen(buffer), 0) <= 0) 
                    {
                        printf("Client disconnected\n");
                        close(k);
                        FD_CLR(k, &master); // Remove from master set
                        FD_CLR(k, &stats_clients); 
                    }
                }
                k++;
            }
        }


        //update: Handle multiple clients
        read_fds = master;
        struct timeval Time; 
        Time .tv_sec = 1; // Set timeout to 1 second
        Time .tv_usec = 0;
        if (select(fdmax + 1, &read_fds, NULL, NULL, &Time) < 0) 
        {
            printf("Select failed");
            return 1;
        }
        else
        {
            int k = 0;
            while(k <= fdmax)
            {
                if(FD_ISSET(k, &read_fds) == 0)
                {
                    k++; 
                    continue; 
                }

                if(k == dev_server)
                {
                    // WAIT FOR CLIENT (runs once per client)
                    // 4. accept a connection — this BLOCKS until someone connects
                    int client_server;
                    if ((client_server = accept(dev_server, NULL, NULL)) < 0) 
                    {
                        printf("Accept failed");
                        k++; 
                        continue;
                    }

                    FD_SET(client_server, &master); // Add to master set
                    if (client_server > fdmax)
                    {
                        fdmax = client_server; // Update fdmax
                    }
                }

                else
                {
                    if(FD_ISSET(k, &stats_clients))
                    {
                        close(k); 
                        FD_CLR(k, &master); 
                        FD_CLR(k, &stats_clients); 
                        k++; 
                        continue; 
                    }
                    // (update) 5. read what the browser is requesting
                    char request[1024] = {0};
                    int bytes = recv(k, request, sizeof(request), 0);
                    if(bytes <= 0)
                    {
                        close(k); 
                        FD_CLR(k, &master); 
                        k++; 
                        continue; 
                    }
 
                    // (update) IF browser wants the HTML page — send home_page.html from disk
                    if (strstr(request, "GET / ")) 
                    {
                        // open home_page.html from disk
                        FILE *f = fopen("home_page.html", "r");
                        if (f == NULL) 
                        {
                            printf("Failed to open home_page.html\n");
                            close(k);
                            FD_CLR(k, &master); // Remove from master set
                            k++; 
                            continue;
                        }
 
                        // (update) send HTML header first so browser knows its receiving an HTML file
                        char html_header[] = "HTTP/1.1 200 OK\r\n" "Content-Type: text/html\r\n" "Access-Control-Allow-Origin: *\r\n\r\n";
                        send(k, html_header, strlen(html_header), 0);
 
                        // (update) read and send the file contents in chunks
                        char filebuf[4096];
                        size_t bytes_read;
                        while ((bytes_read = fread(filebuf, 1, sizeof(filebuf), f)) > 0) 
                        {
                            send(k, filebuf, bytes_read, 0);
                        }
                        fclose(f);
                        close(k);
                        FD_CLR(k, &master); // Remove from master set
                    }
 
                    // (update) IF browser wants the stats data — send JSON stats continuously
                    else if (strstr(request, "GET /stats")) 
                    {
                        // send http header once to client so it can accept the following data when sent
                        char http_header[] = "HTTP/1.1 200 OK\r\n" "Content-Type: application/json\r\n" "Access-Control-Allow-Origin: *\r\n\r\n";
                        send(k, http_header, strlen(http_header), 0);
                        FD_SET(k, &stats_clients); 
                    }

                    else{
                        close(k); 
                        FD_CLR(k, &master); 
                    }
                }
                k++; 
            }
        }
    }
 
    // close socket
    close(dev_server);
    return 0; //return to indicate success
}
