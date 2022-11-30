#ifndef IPC_H_
#define IPC_H_

#include <iostream>
#include <utility>
#include "file-handler.h"

enum class IPCProtocol
{
	NONE, QUEUE, HELP, PIPE, SHARE, 
};


enum class IPCMode
{
	SEND_MODE, RECEIVE_MODE
};


struct ipc_info{
    IPCMode mode{};
    IPCProtocol protocol{};
	char *file_name;
    char *method_name;
};


class IPC
{   
    protected:
        ipc_info info{};
		FileHandler file_handler{};

	public:

        explicit IPC(ipc_info options) : info(options)
		{};
		virtual void init() = 0;
		virtual void transfer() = 0;
		 ~IPC() = default;
};


#endif