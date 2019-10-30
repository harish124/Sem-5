#include<stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<strings.h>
#include<string.h>
#include <stdlib.h>
#include<unistd.h>
void createSocket(int *sockfd)
{
	*sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(*sockfd<0)
	{
		perror("\nSocket not created\n");
	}
}

void initServer(struct sockaddr_in *servaddr)
{
	bzero(servaddr,sizeof(*servaddr));
	(*servaddr).sin_family = AF_INET;
	(*servaddr).sin_addr.s_addr = inet_addr("127.0.0.1");
	(*servaddr).sin_port = htons(8888);	
}


int main()
{
	int sockfd=0;
	struct sockaddr_in servaddr;
	createSocket(&sockfd);
	initServer(&servaddr);	
	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	int n=0;

	char *buff;
	buff = (char *)malloc(100*sizeof(buff));

	while(strcmp(buff,"end")!=0)
	{
		printf("\n(You): ");
		gets(buff);
		n = write(sockfd,buff,strlen(buff));
		buff[n]='\0';

		n = read(sockfd,buff,sizeof(buff));

		printf("\nClient: %s\n", buff);	
	}
	
}