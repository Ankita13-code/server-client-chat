#include<iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#define PORT 1500
using namespace std;


const char *server_message = "Welcome!\n";



int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread, client_socket[10], activity, i, max_clients = 10,
    max_sd, sd;
    fd_set readfds; //set of socket file descriptors
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    //initialise all client_socket[] to 0   
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }  

    // Creating server socket file descriptor 
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server socket created" << endl;

    // Forcefully attaching socket to the port 8000
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // binding the socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    cout << "Waiting for a client to connect....." << endl;

    while(true){
        //clear the socket fd set
        FD_ZERO(&readfds);

        //add server_fd to the above set
        FD_SET(server_fd, &readfds);   
        max_sd = server_fd;

        //add child sockets  to the above set
        for ( i = 0 ; i < max_clients ; i++)   
        {   
            //socket descriptor  
            sd = client_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }  

        //waiting for an activity on one of the sockets
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            cout << "select error"<< endl;   
        } 
        else
        {
            send(sd , server_message , strlen(server_message) , 0 );
        }

        if(FD_ISSET(server_fd, &readfds))
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
            {
	        perror("accept");
	        exit(EXIT_FAILURE);
	        }

            //displaying the new client connected
            cout<< "New client" << endl;
            cout << "Client No: " << new_socket<<endl;
            cout << "Ip and port " << inet_ntoa(address.sin_addr)<<":" << ntohs 
                  (address.sin_port) << endl;
                  

        //add new socket to the set of socket file descriptors
        for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;  
                    break;   
                }   
            }
        }  

        for(i =0; i < max_clients; i++)
            {
                sd = client_socket[i];

                if(FD_ISSET( sd , &readfds))
                {
                
                if ((valread = read( sd , buffer, 1024)) == 0)   
                {     
                    //Close the socket and mark as 0 in list for reuse  
                    printf("Host disconnected , ip %s , port %d \n" ,  
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    close( sd );
                    client_socket[i] = 0;                   
                }
	                cout << buffer<<endl;
	                
	                           
                
                }
                
            }	    
           	    
	}
    return 0;
}