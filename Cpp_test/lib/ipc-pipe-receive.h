#ifndef IPC_PIPE_RECEIVE_H_
#define IPC_PIPE_RECEIVE_H_

#include <sys/stat.h>
#include <fcntl.h>
#include <climits>
#include <semaphore.h>
#include <cstring>
#include "ipc.h"
#include "file-handler.h"

class IPCPipeReceive : public IPC
{
	protected:
		int pd{0};
		std::streamsize p_msgsize{PIPE_BUF};
		sem_t *sem;
		std::string name;
		std::string sem_name;
		
	public:
		explicit IPCPipeReceive(const ipc_info &info)
			: IPC(info){};
		~IPCPipeReceive();
		void init() override;
		void transfer() override;

};

#endif