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
    (*servaddr).sin_addr.s_addr=inet_addr("127.0.0.1");
    (*servaddr).sin_port=htons(7228);
}


int main(int argc,char **argv)
{
    int len;
    int sockfd,n,r;
    struct sockaddr_in servaddr,cliaddr;

    char str[1000];
    char buff[1024];

    createSocket(&sockfd);
    initServer(&servaddr);

    r=connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("Connect returned %d\n",r);

    while(1==1)
    {
        printf("\nClient: ");
        gets(buff);
        printf("%s",buff);

        n=write(sockfd,buff,sizeof(buff));
        //checking end
        if(strcmp(buff,"end")==0)
        {
            break;
        }
        //

        n=read(sockfd,buff,sizeof(buff));

        printf("\nServer: %s",buff);

        //checking end
        if(strcmp(buff,"end")==0)
        {
            break;
        }
    }


    close(sockfd);
    return 0;
}
