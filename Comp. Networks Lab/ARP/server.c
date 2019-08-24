#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<errno.h>
#include <stdlib.h>
#include <string.h>
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

void makeArpPacket(char *packet,char *ipAddr)
{
    char str[100]="127.0.0.1|00:24:be:c6:ce:a7|";

    strcat(str,ipAddr);
    strcpy(packet,str);

    //printf("\nPacket = %s",packet);
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

    //getInputs
    char ipAddr[50];
    printf("\nEnter the destination IP: \n");
    printf("(IP format: XXX.XXX.XXX.XXX)\n(X belongs to integers)\n");
    gets(ipAddr);
    printf("\nIP addr: %s",ipAddr);
    char packet[100];
    makeArpPacket(packet,ipAddr);
    //

    //start accepting
    len=sizeof(cliaddr);
    newfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);
    //
    printf("\nClient(s) connected !!!!\n");     //There may be more than one client

    strcpy(buff,packet);

    printf("\nServer: Sending Packet %s\n",packet);
    n=write(newfd,packet,sizeof(packet));

    int found=0;

    while(1==1)
    {
        n=read(newfd,buff,sizeof(buff));
        if(strcmp(buff,"end")==0)
        {
            break;
        }
        found=1;
        printf("\nClient: %s",buff);
        strcat(buff,"|10101011");

        n=write(newfd,buff,sizeof(buff));
        printf("\nARP Packet sent!!!\n");
        strcpy(buff,"");

    }

    if(!found)
    {
        printf("\nNo client found with that IP address( %s )\n",ipAddr);
    }
    printf("\nClosing....\n");
    close(sockfd);
    close(newfd);
    return 0;
}
