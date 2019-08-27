#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <errno.h>
#define PORT     8080 
#define MAXLINE 1024 

int createSocket(int *sockfd)
{
    *sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(*sockfd<0)
    {
        perror("\nError in creating socket\n");
    }
}//esocket

void initServer(struct sockaddr_in *servaddr)
{

    bzero(servaddr,sizeof(*servaddr));
    (*servaddr).sin_family=AF_INET;
    (*servaddr).sin_addr.s_addr=INADDR_ANY;
    (*servaddr).sin_port=htons(PORT);
}  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello;//="Hello from cl" ;

    struct sockaddr_in     servaddr; 
      
    createSocket(&sockfd);
    initServer(&servaddr);
    
    
    int n=0;
    printf("\nClient: ");
    gets(hello);
    n=sendto(sockfd,
	(const char *)hello,strlen(hello),
     MSG_CONFIRM,
     (const struct sockaddr *)&servaddr,sizeof(servaddr));
     

    

    close(sockfd);   
    return 0; 
} 
