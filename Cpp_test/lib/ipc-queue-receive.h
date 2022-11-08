#ifndef IPC_QUEUE_RECEIVE_H_
#define IPC_QUEUE_RECEIVE_H_

#include <climits>
#include <utility>
#include <mqueue.h>
#include <time.h>
#include <vector>
#include "ipc-common.h"
#include "file-handler.h"


class ipcQueueReceive : public IPC, public FileHandler
{
	protected:
		mqd_t mqd{};
		mq_attr attr{0, 10, 8192, 0};
		unsigned priority{MQ_PRIO_MAX - 1};
	public:
		explicit ipcQueueReceive(const ipc_info &info)
			: IPC(info),
			  FileHandler(info.file_name, FileMode::WRITE)
		{};
		~ipcQueueReceive();
		void init() override;
		void transfer() override;
};

#endif