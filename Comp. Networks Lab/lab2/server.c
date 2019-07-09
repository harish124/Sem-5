#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

void receive(int newfd,char *buff)
{
	read(newfd,buff,2000);
	printf("\nReceived Message is: %s\n",buff);
	
	FILE *fp;
	fp=fopen("ReceivedFile.txt","w");
		
	fwrite(buff,2000,1,fp);
	
	fclose(fp);
	
}

int main(int argc,char **argv)
{

int len;
int sockfd,newfd,n;
struct sockaddr_in servaddr,cliaddr;
char buff[2024];
char str[1000];


sockfd=socket(AF_INET,SOCK_STREAM,0);
if(sockfd<0)
perror("cannot create socket");
else
{
	printf("\nSocket created");
}

bzero(&servaddr,sizeof(servaddr));

servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=INADDR_ANY;
servaddr.sin_port=htons(7220);

if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
perror("Bind error");

listen(sockfd,2);


len=sizeof(cliaddr);
newfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);

//Receiving the message
receive(newfd,buff);



close(sockfd);
close(newfd);
return 0;
}
