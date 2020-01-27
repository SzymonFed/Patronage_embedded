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

void find_word(char *pattern, char *line,char* position);

int main()
{
    
    struct sockaddr_in server;
    
    int server_socket= socket(AF_INET,SOCK_STREAM,0);
    socklen_t socketlen ;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr=inet_addr(SERVER_IP);
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

        
        const char delimiter[] = " ";
        int client_socket;
        printf("Waiting for connection...\n");

        client_socket = accept( server_socket,(struct sockaddr *) &client, &socketlen);
        
        if (client_socket<0)
        {
            perror( "accept() ERROR" );
            continue;
        }
        
        
        if (recv(client_socket,&buffer,sizeof(buffer),0)<=0)
        {
            perror( "recv() ERROR" );
            shutdown (client_socket,SHUT_RDWR);
            
            continue;
        }
        
        
        pattern = strtok(buffer,delimiter);
        if (pattern)
            line = strtok(NULL,delimiter);
        else 
        {
            printf("ERROR: invalid string.\n");
            shutdown (client_socket,SHUT_RDWR);
            continue;
        }
        if(!line)
        {
            printf("ERROR: invalid string.\n");
            shutdown (client_socket,SHUT_RDWR);
            continue;
        }

        find_word(pattern,line,buffer);

        if( send( client_socket,buffer, strlen(buffer), 0) <=0)
            {
                perror("send() ERROR");
                shutdown (client_socket,SHUT_RDWR);
            }

        shutdown (client_socket,SHUT_RDWR);
    }
    shutdown(server_socket,SHUT_RDWR);
}

void find_word(char *pattern, char *line, char* position)
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
        sprintf(position,"%d",-1);
    else
        sprintf(position,"%d",j-pattern_length);
    
}

