#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

void sendMsg(int sockfd)
{
	char fname[100];
	printf("\nEnter any filename: ");
	gets(fname);
	int fd=open(fname,O_RDONLY,0);

	if(fd<0)
	{

		perror("Error reading file\n");
	}
	else
	{
		char buff[50];

		int nb=0;   //no. of bytes read.

		while(nb=read(fd,buff,10))       //reading 10 bytes at a time
        {
            write(sockfd,buff,nb);
            printf("\nBuffer Contents: %s\n",buff);
        }

        strcpy(buff,"\0");
        //printf("\nLast Buffer Content: %s\n",buff);
        write(sockfd,buff,sizeof(buff));
		printf("\nMsg. sent successfully!\n");

	}
}

int main(int argc,char **argv)
{
	int len;
	int sockfd,n,r;
	struct sockaddr_in servaddr,cliaddr;

	char str[1000];
	char buff[2024];

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		perror("cannot create socket");

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port=htons(7220);


	r=connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	printf("connect returned %d\n",r);

	//Sending Message
	sendMsg(sockfd);


	close(sockfd);
	return 0;
}
