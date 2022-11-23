#include "ipc-queue-receive.h"
#include <ctime>
#include <iostream>

ipcQueueReceive::~ipcQueueReceive()
{
	mq_close(this->mqd);
	mq_unlink(this->info.method_name);
}

void ipcQueueReceive::init()
{
	this->file_handler.setup_file(info.file_name, FileMode::WRITE);
	std::cout << "Waiting for sender" << std::endl;
	do
	{
		this->mqd = mq_open(this->info.method_name, O_RDONLY, 0660, &(this->attr));
		int i = 0;
		i++;
		if (i == 10)
		{
			throw std::runtime_error("ERROR: Fail to open receive queue.");
		}
	} while (this->mqd == -1);

	if (this->mqd == -1){
		throw std::runtime_error("ERROR: Fail to open receive queue. ");
	}
	else{
		std::cout << "/dev/mqueue" << this->info.method_name << " is opened." << std::endl;
	}
	this->file_handler.open_file();
}

void ipcQueueReceive::transfer()
{
	struct timespec	ts;
	std::vector<char> buffer(this->attr.mq_msgsize);
	long read_bytes{0};
	std::cout << "Waiting for new file..." << std::endl;
	errno = 0; // clear error number
	
	do
	{
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 7; /* set timeout for 7 seconds */
		ts.tv_nsec = 0; /* Invalid */
		errno = 0;

		read_bytes = mq_timedreceive(this->mqd, buffer.data(), this->attr.mq_msgsize, nullptr, &ts);
		if (read_bytes <= 0 && errno != ETIMEDOUT)
		{
			throw std::runtime_error("ERROR: mq_timedreceive failed to receive file. ");
		}else if (read_bytes > 0) {
			this->file_handler.write_file(buffer, read_bytes);
		}
	}while (read_bytes > 0);
	
	std::cout << "Received data size: " << this->file_handler.get_file_size() << " byte(s)" << std::endl;
}
