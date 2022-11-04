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
		this->mqd = mq_open(this->info.method_name, O_RDONLY | O_NONBLOCK, 0660, &(this->attr));
	} while (this->mqd == -1);

	if (this->mqd == -1)
		throw std::runtime_error("ERROR: Fail to open receive queue. ");
	else
		std::cout << "/dev/mqueue" << this->info.method_name << " is opened." << std::endl;

	this->open_file();
}

void ipcQueueReceive::transfer()
{
	std::vector<char> buffer(this->attr.mq_msgsize);
	long read_bytes{0};
	std::cout << "Waiting for new file..." << std::endl;
	errno = 0; // clear error number
	while (errno != EAGAIN )
	{
		errno = 0; // clear error number
		read_bytes = mq_receive(this->mqd, buffer.data(), this->attr.mq_msgsize, nullptr);
		if (read_bytes > 0)
		{
			std::cout << "read_bytes: " << read_bytes << std::endl;
			this->write_file(buffer, read_bytes);
		}
	}

	std::cout << "Received data: " << this->get_file_size() << " byte(s)" << std::endl;
}
