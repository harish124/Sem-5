#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char **argv)
{
int len;
int sockfd,n,r;
struct sockaddr_in servaddr,cliaddr;

char str[1000];
char buff[1024];

sockfd=socket(AF_INET,SOCK_STREAM,0);
if(sockfd<0)
	perror("cannot create socket");

bzero(&servaddr,sizeof(servaddr));

servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr(argv[1]);
servaddr.sin_port=htons(7777);


r=connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
printf("connect returned %d\n",r);
//Sending Message
printf("Enter the message: ");
gets(buff);

n=write(sockfd,buff,sizeof(buff));
printf("\nMsg sent successfully!");

close(sockfd);
return 0;
}
