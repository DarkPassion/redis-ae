#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ae.h"
#include "anet.h"


char myerr[ANET_ERR_LEN] = {0};
int read_times = 0;

void writeProc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask);
void client_read_proc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask);
void acceptProc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask);

//typedef void aeFileProc(struct aeEventLoop *eventLoop, int fd, void *clientData, int mask);

void writeProc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask)
{
	char* ptr = (char*)clientdata;

	int len = strlen(ptr);
	printf("writeProc -- eventLoop %p fd %d clientdata %p mask %d len %d\n", eventLoop, fd, clientdata, mask, len);
	
	anetWrite(fd, ptr, len);
	free(ptr);
	aeDeleteFileEvent(eventLoop, fd, AE_WRITABLE);
}

void client_read_proc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask)
{
	printf(" client_read_proc -- fd %d  read_times %d\n",  fd, read_times++);
	char read_buff[1024] = {0};

	int n_read = 0;

	n_read = recv(fd, read_buff, sizeof(read_buff), 0);
	//n_read = anetRead(fd, read_buff, sizeof(read_buff) -1);
	
	if (n_read == 0) {
		printf("client socket close = \n");
		aeDeleteFileEvent(eventLoop, fd, AE_READABLE);
		close(fd);
		fd = 0;
		return;
	}

	printf("n_read == %d  %s \n", n_read, read_buff);
	if (n_read <= 0) {
		return;
	}
	

	char* writebuff = (char*)malloc(n_read);
	memset(writebuff, 0, n_read);
	memcpy(writebuff, read_buff, n_read);
	aeCreateFileEvent(eventLoop, fd, AE_WRITABLE, writeProc, writebuff);
}


void acceptProc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask)
{

	printf(" eventLoop %p fd %d clientData %p int mast %d \n", eventLoop, fd, clientdata, mask);
	char ip[20] = {0};
	int port = 0;
	int clientfd = anetTcpAccept(myerr, fd, ip, &port);

	if (clientfd == AE_ERR) {
		printf("error acceptProc -- \n");
		return;
	}

	printf("client info - ip %s port %d \n", ip, port);
	
	anetEnableTcpNoDelay(myerr, clientfd);
	
	int ret = anetNonBlock(myerr, clientfd);
	if (ret == ANET_OK) {
		printf("anetNonBlock-- succ\n");
	}
	aeCreateFileEvent(eventLoop, clientfd, AE_READABLE, client_read_proc, eventLoop);
}

int main()
{
	aeEventLoop* el = aeCreateEventLoop(1024);
	int fd = anetTcpServer(myerr, 8080, "0.0.0.0");

	aeCreateFileEvent(el, fd, AE_READABLE, acceptProc, el);


	aeMain(el);


	return 0;
}