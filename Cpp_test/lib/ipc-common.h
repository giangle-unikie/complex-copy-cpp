#ifndef IPC_COMMON_H_
#define IPC_COMMON_H_

#include <iostream>
#include <utility>

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
	int fflag;
	int qflag;
	int pflag;
	int sflag;
    int hflag;
	
};


class IPC
{   
    protected:
        ipc_info info{};

	public:

        explicit IPC(ipc_info options) : info(std::move(options))
		{};
		virtual void init() = 0;
		virtual void transfer() = 0;
		 ~IPC() = default;
};


#endif