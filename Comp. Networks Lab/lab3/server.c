#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<errno.h>
#define PORT     5000
#define MAXLINE 1024

// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello ;
    hello=(char * )malloc(100*sizeof(char));
    struct sockaddr_in servaddr, cliaddr;

    printf("\nLine 1 before socket");
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("\nLine 1 after socket");

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    printf("\nLine 3 before bind");
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("\nBind Success\n");
    }
    printf("\nLine 4 after bind");
    int len, n;
    while(strcmp(buffer,"end")!=0)
    {
    	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
	    buffer[n] = '\0';
	    printf("Client : %s\n", buffer);
	    printf("\nYou: ");
	    gets(hello);
	    if(strcmp("end",hello)==0)
		{
			break;
		}
	    sendto(sockfd, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
		    len);
    }

    close(sockfd);

    return 0;
}
