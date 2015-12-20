# redis-ae

 redis-2.6.17.tar.gz


## quick start

 ```C

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ae.h"
#include "anet.h"

 void writeProc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask)
{
	/* write process */

}

 void acceptProc(struct aeEventLoop* eventLoop, int fd, void* clientdata, int mask)
{

	/* accecpt process */

}

int main()
{
	aeEventLoop* el = aeCreateEventLoop(1024);
	int fd = anetTcpServer(myerr, 8080, "0.0.0.0");

	aeCreateFileEvent(el, fd, AE_READABLE, acceptProc, el);


	aeMain(el);

	aeDeleteEventLoop(el);

	return 0;
}


 ```

## MQTT client mqttc 

基于redis-ae event-loop Mqtt client

https://github.com/emqtt/mqttc
