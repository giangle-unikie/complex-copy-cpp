#ifndef IPC_QUEUE_SEND_H_
#define IPC_QUEUE_SEND_H_

#include <mqueue.h>
#include <climits> 
#include "ipc-common.h"
#include "file-handler.h"

class ipcQueueSend : public IPC, public FileHandler
{
	protected:
		mqd_t mqd{};
		mq_attr attr{0, 10, 8192, 0};
		unsigned priority{MQ_PRIO_MAX - 1};
	public:
		explicit ipcQueueSend(const ipc_info &info)
			: IPC(info),
			  FileHandler(info.file_name, FileMode::READ)
		{};
		~ipcQueueSend();
		void init() override;
		void transfer() override;
};

#endif