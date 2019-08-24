#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<errno.h>

#define backlog 2

int createSocket(int *sockfd)
{
    *sockfd=socket(AF_INET,SOCK_STREAM,0);
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
    (*servaddr).sin_port=htons(7228);
}

void bindAndListen(int *sockfd,struct sockaddr_in *servaddr)
{
    if(bind(*sockfd,(struct sockaddr*)servaddr,sizeof(*servaddr))<0)     //bind the socket with the local protocol addr
    perror("Bind Error");                                               //which is a combination of ip addr and port no.

    listen(*sockfd,backlog);     //sets the state of the socket here sockfd to listening state. backlog --> no. of clients server can listen to.
}

int main(int argc,char **argv)
{

    int len;
    int sockfd,newfd,n,r;
    struct sockaddr_in servaddr,cliaddr;
    char buff[1024];
    char str[1000];

    createSocket(&sockfd);
    initServer(&servaddr);
    bindAndListen(&sockfd,&servaddr);

    //start accepting
    len=sizeof(cliaddr);
    newfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);
    //

    while(1==1)
    {
        n=read(newfd,buff,sizeof(buff));
        printf("\nClient: %s",buff);
        if(strcmp(buff,"end")==0)
        {
            break;
        }
        printf("\nServer: ");
        gets(buff);
        printf("%s\n",buff);
        n=write(newfd,buff,sizeof(buff));
        if(strcmp(buff,"end")==0)
        {
            break;
        }

    }



    close(sockfd);
    close(newfd);
    return 0;
}
