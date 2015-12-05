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
void readProc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask);


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

void readProc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask)
{
	printf(" client_read_proc -- fd %d  read_times %d\n",  fd, read_times++);
	char read_buff[1024] = {0};

	int n_read = 0;

	//n_read = anetRead(fd, read_buff, sizeof(read_buff) -1);
	n_read = recv(fd, read_buff, sizeof(read_buff), 0);

	
	
	if (n_read == 0) {
		printf("server close socket -- \n");
		aeDeleteFileEvent(eventLoop, fd, AE_READABLE);
		close(fd);
		return;
	}
	
	if (n_read == -1) {
		return ;
	}

	printf("n_read == %d %s \n", n_read, read_buff);
	char* writebuff = (char*)malloc(n_read);
	memset(writebuff, 0, n_read);
	memcpy(writebuff, read_buff, n_read);
	aeCreateFileEvent(eventLoop, fd, AE_WRITABLE, writeProc, writebuff);
}


int main()
{
	aeEventLoop* el = aeCreateEventLoop(1024);

	int fd = anetTcpConnect(myerr, "127.0.0.1", 8080);

	char writebuff[20] = {0};
	const char * ptr = "hello worold !";
	sprintf(writebuff, ptr, strlen(ptr));
	anetWrite(fd, writebuff, strlen(ptr));

	anetNonBlock(myerr, fd);
	aeCreateFileEvent(el, fd, AE_READABLE, readProc, el);

	printf("main --- \n");
	aeMain(el);


	return 0;
}