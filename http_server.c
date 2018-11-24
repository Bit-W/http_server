#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
int main()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	if(sockfd < 0)
	{
		perror("socket error");
		return -1;
	}

	struct sockaddr_in addr;

	socklen_t len = 0;
	len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	//  addr.sin_addr.s_addr = inet_addr("192.268.179.128");
	addr.sin_addr.s_addr = inet_addr("0");

	int ret = bind(sockfd,(struct sockaddr*)&addr,len);
	if(ret < 0)
	{
		perror("bind error");
		return -1;
	}

	ret = listen(sockfd,5);
	if(ret < 0)
	{
		perror("listen error");
		return -1;
	}
	while(1)
	{
		struct sockaddr_in cli_sockfd;
		int newsockfd = accept(sockfd,(struct sockaddr*)&cli_sockfd,&len);
		if(newsockfd < 0)
		{
			perror("accept error");
			continue;
		}

		char buf[1024] = {0};
		ret = recv(newsockfd,buf,1023,0);
		if(ret <= 0)
		{
			perror("recv error");
			continue;
		}
		printf("[ %s ]\n",buf);
		//整体的http响应应包括
		//1.首行
		//HTTP/1.1 200 OK
		//2.头信息  
		//Content-Length;
		//Content-Type: text/html; charset=UTF-8
		//3.空行
		//4.正文 ----rsp

		char* rsp = "<hl>hello world</hl>";

		memset(buf,0x00,1024);
		sprintf(buf,"%s\r\n%s%d\r\n%s\r\n%s\r\n\r\n%s",
				"HTTP/1.1 302 Found",
				"Content-Length: ",strlen(rsp),
				"Content-Type: text/html",
				"Location: http://www.baidu.com/",rsp);
		send(newsockfd,buf,strlen(buf),0);

		//      close(newsockfd);
	}
	close(sockfd);
	return 0;
}

