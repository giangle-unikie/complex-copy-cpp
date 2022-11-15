#ifndef IPC_QUEUE_RECEIVE_H_
#define IPC_QUEUE_RECEIVE_H_

#include <climits>
#include <utility>
#include <mqueue.h>
#include <time.h>
#include <vector>
#include "ipc.h"
#include "file-handler.h"


class ipcQueueReceive : public IPC
{
	protected:
		mqd_t mqd{};
		mq_attr attr{0, 10, 8192, 0};
		unsigned priority{MQ_PRIO_MAX - 1};
		FileHandler file_handler;
	public:
		explicit ipcQueueReceive(const ipc_info &info)
			: IPC(info),
			  file_handler(info.file_name, FileMode::WRITE)
		{};
		~ipcQueueReceive();
		void init() override;
		void transfer() override;
};

#endif