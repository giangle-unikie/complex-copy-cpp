#ifndef IPC_QUEUE_SEND_H_
#define IPC_QUEUE_SEND_H_

#include <mqueue.h>
#include <climits> 
#include <time.h>
#include <vector>
#include "ipc.h"

class ipcQueueSend : public IPC
{
	protected:
		mqd_t mqd{};
		mq_attr attr{0, 10, 8192, 0};
		unsigned priority{MQ_PRIO_MAX - 1};
		
	public:
		explicit ipcQueueSend(const ipc_info &info)
			: IPC(info){};
		~ipcQueueSend();
		void init() override;
		void transfer() override;
};

#endif