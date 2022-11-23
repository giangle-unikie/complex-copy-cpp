#ifndef IPC_QUEUE_RECEIVE_H_
#define IPC_QUEUE_RECEIVE_H_

#include <climits>
#include <utility>
#include <mqueue.h>
#include <time.h>
#include <vector>
#include "ipc.h"


class ipcQueueReceive : public IPC
{
	protected:
		mqd_t mqd{};
		mq_attr attr{0, 10, 8192, 0};
		unsigned priority{MQ_PRIO_MAX - 1};
		
	public:
		explicit ipcQueueReceive(const ipc_info &info)
			: IPC(info){};
		~ipcQueueReceive();
		void init() override;
		void transfer() override;
};

#endif