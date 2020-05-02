#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#define PORT 1500

using namespace std;

int main ()
{
    int client =0;
    int valread;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize] = {0};
    const char* ip = "127.0.0.1";
    const char *hello = "Hello from client"; 

    struct sockaddr_in server_addr, address;

    //creating a socket

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0)
    {
        cerr << "Error creating socket..." << endl;
        exit(1);
    }

    cout<< "Client socket created" << endl;
    memset(&server_addr, '0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)  
    { 
       cerr << "\nInvalid address/ Address not supported \n" << endl; 
        return -1; 
    } 

    cout << "Connection about to be set up"<<endl;

    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    { 
        cout << "\nConnection Failed" << endl; 
        return -1; 
    } 
    send(client, hello, strlen(hello), 0);
    int t = 100;

    while(t--){
        cin.getline(buffer, bufsize);
    send(client , buffer , sizeof(buffer) , 0 ); 
   /* valread = read( client , buffer, bufsize); 
    cout << buffer << endl;*/
    }
    

       return 0;



}