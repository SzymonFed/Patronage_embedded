#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define MAX_MSG_LEN 4095
#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define MAX_CONNECTION 10

int find_word(char *pattern, char *line);

int main()
{
    
    struct sockaddr_in server;
    
    const int server_socket= socket(AF_INET,SOCK_STREAM,0);
    socklen_t socketlen ;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr=htons(SERVER_IP);
    socketlen = sizeof(server);
    
    
    if( inet_pton( AF_INET, SERVER_IP, & server.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
    
    if(server_socket<0)
    {
        perror( "socket() ERROR" );
        exit( 2 );
    }
   
    if(bind( server_socket,( struct sockaddr * ) & server, sizeof( server ))< 0)
    {
        perror( "bind() ERROR" );
        exit( 3 );
    }
    if ( listen (server_socket, MAX_CONNECTION)<0)
    {
        perror( "listen() ERROR" );
        exit( 4 );
    }
    while(1)
    {
        
        
        struct sockaddr_in client;
        char buffer[MAX_MSG_LEN];
        char *pattern;
        char *line;
        int position;
        const char delimiter[] = " ";
        int clientSocket;
        printf("Waiting for connection...\n");

        clientSocket = accept( server_socket,(struct sockaddr *) &client, &socketlen);
        
        if (clientSocket<0)
        {
            perror( "accept() ERROR" );
            continue;
        }
        
        if ( recv(clientSocket,&buffer,sizeof(buffer),0)<=0)
        {
            perror( "recv() ERROR" );
            exit( 5 );
        }
        

        pattern = strtok(buffer,delimiter);
        if (pattern)
            line = strtok(NULL,delimiter);
        else 
        {
            printf("ERROR: invalid string.\n");
            exit(0);
        }
        if(!line)
        {
            printf("ERROR: invalid string.\n");
            exit(0);
        }
        position = find_word(pattern,line);

        printf("Pattern:%s\n Line: %s\n Position of pattern: %d\n",pattern,line,position);

        shutdown (clientSocket,SHUT_RDWR);
    }
    shutdown(server_socket,SHUT_RDWR);
}

int find_word(char *pattern, char *line)
{
    int i=0, j = 0;
    int pattern_length, line_length;
    pattern_length = strlen(pattern);
    line_length = strlen(line);

    while(i<pattern_length && j<line_length)
    {
        if (pattern[i]!=line[j])
            i=0;
        else
            i++;
        j++;
    }

    if (i< (pattern_length-1))
        return -1;
    else
        return j-pattern_length;
}

