#include "ipc-queue-send.h"
#include <ctime>
#include <iostream>
#include <unistd.h>

ipcQueueSend::~ipcQueueSend()
{
	mq_close(this->mqd);
	mq_unlink(this->info.method_name);
}

void ipcQueueSend::init()
{
	this->file_handler.setup_file(info.file_name, FileMode::READ);
	// remove old queue name (if any)
	mq_unlink(this->info.method_name);
	errno = 0; // clear errno
	this->mqd = mq_open(this->info.method_name, O_CREAT | O_EXCL | O_WRONLY, 0660, &(this->attr));

	if (this->mqd == -1)
	{
		throw std::runtime_error(std::string("ERROR: open queue send name ") + this->info.method_name);
	}
	else
	{
		std::cout << this->info.method_name << " is opened." << std::endl;
	}
	this->file_handler.open_file();
}

void ipcQueueSend::transfer()
{
	struct timespec ts;
	std::cout << "transfer\n";
	long mq_send_return_value{0};
	long read_bytes{0};
	std::vector<char> buffer(this->attr.mq_msgsize);
	unsigned long long total_sent_bytes{0};
	unsigned long long file_size = this->file_handler.get_file_size();
	if (file_size == 0)
	{
		throw std::runtime_error("ERROR: File size = 0.");
	}

	std::cout << "Sending..." << std::endl;

	while (total_sent_bytes < file_size)
	{
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 3; /* set timeout for 3 seconds */
		ts.tv_nsec = 0; /* Invalid */

		this->file_handler.read_file(buffer, this->attr.mq_msgsize);
		read_bytes = this->file_handler.get_read_bytes();

		if (read_bytes > 0)
		{
			mq_send_return_value = mq_timedsend(this->mqd, buffer.data(), read_bytes,
												this->priority, &ts);
			if (mq_send_return_value == 0)
			{
				total_sent_bytes += read_bytes;
			}
			else
			{
				throw std::runtime_error(std::string("ERROR: mq_send(): "));
			}
		}
	}

	std::cout << "Sent data size: " << total_sent_bytes << std::endl;

	if (total_sent_bytes == file_size)
	{
		std::cout << "Waiting for client to pick up..." << std::endl;
		int time_wait{0};
		size_t is_empty = 0;
		do
		{
			mq_getattr(this->mqd, &(this->attr));
			is_empty = this->attr.mq_curmsgs;
			sleep(1);
			time_wait++;
			std::cout << time_wait << std::endl;
			if (time_wait == 10)
			{
				throw std::runtime_error("ERROR: Fail time out send queue.\n");
			}

		} while (is_empty != 0);

		std::cout << "Client picked it up." << std::endl;
	}
	else
	{
		throw std::runtime_error("ERROR: The size of Total send file is not equal to File size.");
	}
}
