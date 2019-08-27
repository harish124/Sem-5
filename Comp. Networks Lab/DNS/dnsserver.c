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
// Driver code 

int checkIP(char *ip,char table[20][20],int domainOrIp)
{														//table[evenNo]  --> domain name
														//table[oddNo]   --> ip address
	
	for (int i = domainOrIp;  i < 20;  i += 2)
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
		
		
		flag=checkIP(ip,table,1);		
		if(flag==1)
		{
			printf("\nIp already exists\n");
			flag=0;			
			goto GETIP;
		}
		
		strcpy(table[counter],dname);
		strcpy(table[counter+1],ip);
		counter+=2;
		
	}
	
	
}//emakeTable
void displayTable(char table[20][20])
{
	int i=0;
	printf("\nDomain Name\tIP address\n");
	
	for (i = 0; i < 19; i += 2)
	{
		if(table[i]==NULL)
		{
			
			continue;
		}	
		printf("%s\t%s\n",table[i],table[i+1]);
		
	}
}
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello ;
    hello=(char * )malloc(100*sizeof(char));

    struct sockaddr_in servaddr, cliaddr; 
      
    bzero(&cliaddr,sizeof(cliaddr));
    
    createSocket(&sockfd);
    initServer(&servaddr);
    bindAndListen(&sockfd,&servaddr);
      
	int n=0,len;
	char table[20][20];
	
	makeTable(table);
	displayTable(table);
 	
 	n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len); 
    buffer[n]='\0';
	printf("\nServer: %s\n",buffer);              
    
    close(sockfd);
      
    return 0; 
} 
