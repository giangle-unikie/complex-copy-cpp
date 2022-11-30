#ifndef IPC_PIPE_SEND_H_
#define IPC_PIPE_SEND_H_

#include <sys/stat.h>
#include <fcntl.h>
#include <climits>
#include "ipc.h"
#include "file-handler.h"

class IPCPipeSend : public IPC
{
	protected:
		int pd{0};
		std::streamsize p_msgsize{PIPE_BUF};
		
	public:
		explicit IPCPipeSend(const ipc_info &info)
			: IPC(info){};
		~IPCPipeSend();
		void init() override;
		void transfer() override;
};

#endif 