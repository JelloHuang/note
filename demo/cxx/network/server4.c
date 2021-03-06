/* server.c */
/*cas:多线程并发服务器:安全性问题
 * 症状：当只有一个客户连接时，可以正常工作
 * 当此时有另外一个客户连接时，就会出现问题，
 * 非线程安全函数使用了静态变量index
 * 当连接建立后，服务器调用pthread_create()函数产生新的线程
 * 新的线程处理客户请求 最后关闭链接套接字并退出进程
 * 服务器等候客户连接，一旦有连接则显示客户的地址，然后接受来自客户的的信息（字符串）。每当收到一个字符串，则显示该字符串
 * 并将字符串反转，并将反转的结果发回给客户，之后继续等待接受客户的信息直至该客户关闭连接
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 1234
#define BACKLOG 5
#define MAXDATASIZE 1000

void process_cli(int connectfd, struct sockaddr_in client);
//非线程安全函数
void savedata(char *recvbuf,int len,char *cli_data);
void* start_routine(void *arg);

typedef struct ARG{
	int connfd;
	struct sockaddr_in client;
}ARG_t;

int main(void) {
	int listenfd, connectfd;
	pthread_t thread;
	ARG_t *arg;
	struct sockaddr_in server, client; //internet socket address for server and client
	socklen_t sin_size;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		/*handle exception*/
		perror("creating socket failed");
		exit(1);
	}

	int opt = SO_REUSEADDR;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	if ((bind(listenfd, (struct sockaddr *) &server, sizeof(struct sockaddr)))
			== -1) {
		perror("bind error");
		exit(1);
	}

	if ((listen(listenfd, BACKLOG)) == -1) {
		perror("listen error");
		exit(1);
	}
	sin_size = sizeof(struct sockaddr_in);

	printf("Accepting connections ...\n");
	while (1) {
		if ((connectfd = accept(listenfd, (struct sockaddr *) &client,
				&sin_size)) == -1) {
			perror("accept() error");
			exit(1);
		}

		//create thread to service the client
		arg=(ARG_t *)malloc(sizeof(ARG_t *));
		arg->connfd=connectfd;
		//memcpy((void *)&arg->client,&client,sizeof(client));
		arg->client=client;
		printf("\n");
		printf("arg address: %p\n",&arg);
		printf("arg connfd address: %p\n",&arg->connfd);
		printf("arg client address: %p\n",&arg->client);
		printf("connectfd address: %p\n",&connectfd);
		printf("client address: %p\n",&client);
		printf("\n");

		if(pthread_create(&thread,NULL,start_routine,(void *)arg) !=0){
			//handle error
			printf("Pthread_create() error");
			exit(1);
		}
	}//end while
	close(listenfd);
}

void process_cli(int connectfd, struct sockaddr_in client) {
	int num;
	char recvbuf[MAXDATASIZE], sendbuf[MAXDATASIZE],cli_name[MAXDATASIZE];
	char cli_data[5000];

	/*print client's ip*/
	printf("you got a connection from %s\n", inet_ntoa(client.sin_addr));

	//print client's name from client
	num=recv(connectfd,cli_name,MAXDATASIZE,0);
	if(num==0){
		close(connectfd);
		printf("client disconnectd");
		return;
	}
	cli_name[num-1]='\0';
	printf("client's name is %s\n",cli_name);

	char *servermessage="you have revevied an empty string";
	char *clientmessage="you have typed an empty string";
	while ((num = recv(connectfd, recvbuf, MAXDATASIZE, 0)) > 0) {

		//if num=1,说明用户直接按下回车键，而没有有效信息
		if(num==1){
			puts(servermessage);
			send(connectfd,clientmessage,strlen(clientmessage),0);
		}
		else {
			recvbuf[num] = '\0';
			printf("Received client message:[%s] %s\n",cli_name, recvbuf);
			savedata(recvbuf,num,cli_data);
			/* Reverse the message*/
			int i = 0;
			for (i = 0; i < num - 1; i++) {
				sendbuf[i] = recvbuf[num - i - 2];
			}
			sendbuf[num - 1] = '\0';
			//send to the client welcome message
			send(connectfd, sendbuf, strlen(sendbuf), 0);
		}
	}
	close(connectfd);
	printf("client(%s) closed connection. User's data: %s\n",cli_name,cli_data);
}

void* start_routine(void* arg){
	ARG_t *info;
	info=(ARG_t *)arg;

	//handle client's requirement
	process_cli(info->connfd,info->client);

	free(arg);
	printf("\n arg is freeed\n");
	pthread_exit(NULL);
	return (void*)NULL;
}

void savedata(char *recvbuf, int len, char *cli_data){
	static int index=0;
	int i=0;
	for (i=0;i<len-1;i++){
		cli_data[index++]=recvbuf[i];
	}
	cli_data[index]='\0';
	printf("\nindex=%d\tcli_data:\t%s\n",index,cli_data);
}
