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

	std::cout << "Waiting for sender" << std::endl;
	do
	{
		this->mqd = mq_open(this->info.method_name, O_RDONLY, 0660, &(this->attr));
	} while (this->mqd == -1);

	if (this->mqd == -1)
		throw std::runtime_error("ERROR: Fail to open receive queue. ");
	else
		std::cout << "/dev/mqueue" << this->info.method_name << " is opened." << std::endl;

	this->open_file();
}

void ipcQueueReceive::transfer()
{
	struct timespec	ts;
	std::vector<char> buffer(this->attr.mq_msgsize);
	long read_bytes{0};
	std::cout << "Waiting for new file..." << std::endl;
	errno = 0; // clear error number
	
	while (read_bytes != -1)
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
			this->write_file(buffer, read_bytes);
		}
	}

	std::cout << "Received data size: " << this->get_file_size() << " byte(s)" << std::endl;
}
