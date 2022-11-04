#ifndef IPC_HANDLER_H_
#define IPC_HANDLER_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <getopt.h>
#include "ipc-common.h"
#include "ipc-queue-receive.h"
#include "ipc-queue-send.h"


class ipcHandler
{   
    private:
        ipc_info info{};
      
	public:
		
        void select_options(IPCMode mode, int argc, char *argv[]);
		const ipc_info &get_options() const;
        IPCProtocol start() const;
         ~ipcHandler() = default;
};



#endif
