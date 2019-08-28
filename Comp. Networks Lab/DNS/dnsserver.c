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
#define backlog 2


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
// Driver code

int checkIP(char *ip,char table[20][20],int domainOrIp)
{														//table[evenNo]  --> domain name
														//table[oddNo]   --> ip address

	for (int i = domainOrIp;  i < 20 && table[i]!=NULL;  i += 2)
	{
		if(strcmp(ip,table[i])==0)
		{
			return 1;
		}
	}
}//echeckIP
int makeTable(char table[20][20])
{														//table[evenNo]  --> domain name
														//table[oddNo]   --> ip address
	int n=2;
    char *dname,*ip;
    dname=(char *)malloc(20*sizeof(char));
    ip=(char *)malloc(20*sizeof(char));
	int flag=0;
	printf("\nEnter the no. of domain names you want to insert: ");
//	scanf("%d",&n);

	int counter=0;
	while(counter<=n)
	{
		DNAME:
		printf("\nEnter domain name: ");
		gets(dname);

		flag=checkIP(dname,table,0);
		if(flag==1)
		{
			printf("\nDomain Name already exists\n");
			flag=0;
			goto DNAME;
		}

		GETIP:
		printf("\nEnter the ip: ");
		gets(ip);

        if(inet_addr(ip)==-1)
        {
            printf("\nInvalid IP\nEnter a valid IP\n");
            goto GETIP;
        }

		flag=checkIP(ip,table,1);
		if(flag==1)
		{
			printf("\nIp already exists\n");
			flag=0;
			goto GETIP;
		}

		dname[strlen(dname)]='\0';
		ip[strlen(ip)]='\0';
		strcpy(table[counter],dname);
		strcpy(table[counter+1],ip);
		strcpy(dname,"");
		strcpy(ip,"");
		counter+=2;

	}


}//emakeTable
void displayTable(char table[20][20])
{
	int i=0;
	printf("\nDomain Name\tIP address\n");

	for (i = 0; i < 19; i += 2)
	{
		printf("%s\t%s\n",table[i],table[i+1]);

	}
}

int receiveMsg(int sockfd,char *buffer,struct sockaddr_in *cliaddr,int *len)
{
    int n=0;
    n = recvfrom(sockfd,
                (char *)buffer, MAXLINE,
                0,                                    //When MSG_CONFIRM/MSG_WAITALL they become same as that of write and read in TCP.//Using 0 here because MSG_COnfirm wont work in windows.
                 ( struct sockaddr *)cliaddr,
                len);
    buffer[n]='\0';
    return n;
}//erecvMsg

void lookUp(char table[20][20],char *dname)
{
    for(int i=0;i<19;i+=2)
    {
        //printf("\nstr = %s\ndname = %s\n%d\n",table[i],dname,strcmp(dname,table[i]));
        if(strcmp(dname,table[i])==0)
        {
            strcpy(dname,table[i+1]);
            return;
        }
    }
    strcpy(dname,"");       //empty the buffer to signal that no value found.
}
void sendMsg(int sockfd,char *hello,struct sockaddr_in *cliaddr)
{

    int n=0;
    n=sendto(sockfd,
	(const char *)hello,strlen(hello),
     0,                                     //When MSG_CONFIRM/MSG_WAITALL they become same as that of write and read in TCP.//Using 0 here because MSG_COnfirm wont work in windows.
     (const struct sockaddr *)cliaddr,sizeof(*cliaddr));
}

void recvDomainName(int sockfd,struct sockaddr_in *cliaddr,int *len,char table[20][20])
{
    int n=0;
    char *buff=(char *)malloc(25*sizeof(char));

    int clients=0;

    while(clients<backlog)
    {
        n=receiveMsg(sockfd,buff,cliaddr,len);
        buff[n]='\0';
        printf("\nClient: Requesting domain : \'%s\'\n",buff);
        lookUp(table,buff);
        sendMsg(sockfd,buff,cliaddr);
        printf("\nReply sent\n");
        clients++;
    }
}

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello ;
    hello=(char * )malloc(100*sizeof(char));

    struct sockaddr_in servaddr, cliaddr;

    createSocket(&sockfd);
    initServer(&servaddr);
    bindAndListen(&sockfd,&servaddr);

	int n=0,len;
	char table[20][20];

	bzero(table,sizeof(table));

	makeTable(table);
	//displayTable(table);

    len = sizeof(cliaddr);

    recvDomainName(sockfd,&cliaddr,&len,table);

    //sendMsg(sockfd,buffer,&cliaddr);

    close(sockfd);

    return 0;
}
