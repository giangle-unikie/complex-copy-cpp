#ifndef IPC_QUEUE_SEND_H_
#define IPC_QUEUE_SEND_H_

#include <mqueue.h>
#include <climits> 
#include <time.h>
#include <vector>
#include "ipc.h"
#include "file-handler.h"

class ipcQueueSend : public IPC
{
	protected:
		mqd_t mqd{};
		mq_attr attr{0, 10, 8192, 0};
		unsigned priority{MQ_PRIO_MAX - 1};
		FileHandler file_handler;
	public:
		explicit ipcQueueSend(const ipc_info &info)
			: IPC(info),
			  file_handler(info.file_name, FileMode::READ)
		{};
		~ipcQueueSend();
		void init() override;
		void transfer() override;
};

#endif