#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT     8080 
#define MAXLINE 1024 

void broadcast(int sockfd,struct sockaddr_in servaddr);
  
char *address[25][25];

void init()
{	
	*address[0]="10.6.12.10";
	*address[1]="10.6.12.9";
	*address[2]="10.6.12.8";
	*address[3]="10.6.12.7";
	*address[4]="10.6.12.6";
	printf("\naddress: %s",*address[0]);	

	
}//einit

void broadcast(int sockfd,struct sockaddr_in servaddr)
{
	int i=0,n=0;
	    int len=0;
	char buffer[25];
	    char *hello="10.6.12.12|Mac:|10.6.12.10" ;
	sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
	
}//ebroadcast  

// Driver code 
int main() { 
    
    init();
    
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello;//="Hello from cl" ;

    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int n, len; 
    
    broadcast(sockfd,servaddr);
      
    while(strcmp(buffer,"end")!=0)
    {
	printf("\nYou: ");
	gets(hello);
	if(strcmp("end",hello)==0)
	{
		break;
	}
	sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
//    	printf("Hello message sent.\n"); 
          
    	n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    	buffer[n] = '\0'; 
    	printf("Server : %s\n", buffer); 
  
    }
    close(sockfd);   
    return 0; 
} 
