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
	// remove old queue name (if any)
	mq_unlink(this->info.method_name);
	errno = 0; // clear errno
	this->mqd = mq_open(this->info.method_name, O_CREAT | O_EXCL | O_WRONLY , 0660, &(this->attr));

	if (this->mqd == -1)
		throw std::runtime_error(std::string("ERROR: open queue send name ") + this->info.method_name );
	else
		std::cout << this->info.method_name << " is opened." << std::endl;

	this->open_file();
}

void ipcQueueSend::transfer()
{
	struct timespec	ts;
	std::cout << "transfer\n";
	long mq_send_return_value{0};
	long read_bytes{0};
	std::vector<char> buffer(this->attr.mq_msgsize);
	unsigned total_sent_bytes{0};
	unsigned long file_size = this->get_file_size();
	if (file_size == 0){
		throw std::runtime_error("ERROR: File size = 0.");
	}
		
	std::cout << "Sending..." << std::endl;
	std::cout << "file_size: " << file_size << std::endl;
	
	while (total_sent_bytes < file_size)
	{
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += 7; /* set timeout for 7 seconds */
		ts.tv_nsec = 0; /* Invalid */
		
		this->read_file(buffer, this->attr.mq_msgsize);
		read_bytes = this->fs.gcount();
		
		if (read_bytes > 0)
		{
			mq_send_return_value = mq_timedsend(this->mqd, buffer.data(), read_bytes,
												   this->priority, &ts);
			std::cout << "mq send: " << mq_send_return_value << std::endl;
			if (mq_send_return_value == 0)
			{
				total_sent_bytes += read_bytes;
				
			}else
			{
				throw std::runtime_error(std::string("ERROR: mq_send(): "));
			}
				
		}
	}

	std::cout << "Sent data size: " << total_sent_bytes << std::endl;

	if (total_sent_bytes == file_size)
	{
		
		size_t is_empty = 0;
		size_t mq_oldmsgs = 0; // store the previous value of mq_curmsgs
		do
		{
			mq_getattr(this->mqd, &(this->attr));
			is_empty = this->attr.mq_curmsgs;
			mq_oldmsgs = is_empty;
		} while (is_empty != 0);

		if (is_empty != 0)
			throw std::runtime_error("ERROR: Timeout. \n");
		else
			std::cout << "Client picked it up." << std::endl;
	}
	else
	{
		throw std::runtime_error("ERROR: Connection lost. The transfer is interrupted.");
	}
}

