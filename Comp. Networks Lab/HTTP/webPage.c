#include<stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<strings.h>
#include<unistd.h>
#include<string.h>
#include <stdlib.h>
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
	(*servaddr).sin_addr.s_addr = inet_addr("216.165.47.12");
	(*servaddr).sin_port = htons(80);	
}


int main()
{
	int sockfd=0;
	struct sockaddr_in servaddr;
	createSocket(&sockfd);
	initServer(&servaddr);
	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

	int n=0;	

	char *buff;
	buff = (char *)malloc(100*sizeof(buff));

	snprintf(buff, 99, "GET / HTTP/1.1\r\n""Host: %s\r\n""\r\n\r\n", "www.NYU.edu");
	if (send(sockfd, buff, strlen(buff), 0) < 0) {
		puts("Send failed!\n");
		return 1;
	}
	puts("Data Sent...\n");

	FILE *fp;

	fp = fopen("webPage.html","w");
	while( (n = read(sockfd,buff,sizeof(buff)) ) > 0)
	{
		buff[n]='\0';	
		fputs(buff,fp);
		//printf("\nClient: %s\n", buff);
		strcpy(buff,"");	
	}
	

	
	fclose(fp);
	// printf("\n(You): \n");
	// gets(buff);
	// n = write(sockfd,buff,sizeof(buff));


	
}