#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<errno.h>
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
void bindAndListen(int *sockfd,struct sockaddr_in *servaddr)
{
    if(bind(*sockfd,(struct sockaddr*)servaddr,sizeof(*servaddr))<0)     //bind the socket with the local protocol addr
    perror("Bind Error");                                               //which is a combination of ip addr and port no.
}

void sendMsg(int sockfd,char *hello,struct sockaddr_in *servaddr)
{

    int n=0;
    n=sendto(sockfd,
	(const char *)hello,strlen(hello),
     0,                                     //When MSG_CONFIRM/MSG_WAITALL they become same as that of write and read in TCP.//Using 0 here because MSG_COnfirm wont work in windows.
     (const struct sockaddr *)servaddr,sizeof(*servaddr));
}
int receiveMsg(int sockfd,char *buffer,struct sockaddr_in *servaddr,int *len)
{
    int n=0;
    n = recvfrom(sockfd,
                (char *)buffer, MAXLINE,
                0,                                    //When MSG_CONFIRM/MSG_WAITALL they become same as that of write and read in TCP.//Using 0 here because MSG_COnfirm wont work in windows.
                 ( struct sockaddr *)servaddr,
                len);
    buffer[n]='\0';
    return n;
}//erecvMsg

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello ;
    hello=(char * )malloc(100*sizeof(char));
    struct sockaddr_in servaddr, cliaddr;

    createSocket(&sockfd);
    initServer(&servaddr);
    bindAndListen(&sockfd,&servaddr);
    int len=sizeof(cliaddr), n;

    while(strcmp(hello,"end")!=0)
    {
    	n = receiveMsg(sockfd,buffer,&cliaddr,&len);
	    buffer[n] = '\0';
	    printf("\nClient : %s\n", buffer);
	    if(strcmp("end",buffer)==0)
		{
			break;
		}
	    printf("You: ");
	    gets(hello);

	    sendMsg(sockfd,hello,&cliaddr);
    }

    close(sockfd);

    return 0;
}
