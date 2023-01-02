#include "ipc-pipe-receive.h"

IPCPipeReceive::~IPCPipeReceive()
{
	close(this->pd);
	unlink(this->info.method_name);
}

void IPCPipeReceive::init()
{
	this->file_handler.setup_file(info.file_name, FileMode::WRITE);
	std::cout << "Waiting for sender... " << std::endl;
	int time_wait{0};
	do
	{
		this->pd = open(this->info.method_name, O_RDONLY, O_NONBLOCK);

		time_wait++;
		sleep(1);
		std::cout << time_wait << std::endl;

	} while (this->pd == -1 && time_wait < 10);
	if (time_wait >= 10)
	{
		throw std::runtime_error("ERROR: Running out of time wait of receiver, 10s \n");
	}

	std::cout << this->info.method_name << " is opened." << std::endl;

	this->file_handler.open_file();
}

void IPCPipeReceive::transfer()
{
	long read_bytes{1};
	unsigned long long total_received_bytes{0};
	std::vector<char> buffer(this->p_msgsize);
	std::cout << "Receiving..." << std::endl;
	while (true)
	{
		read_bytes = read(this->pd, buffer.data(), this->p_msgsize);
		if (read_bytes == 0)
		{
			break;
		}

		if (read_bytes > 0)
		{
			this->file_handler.write_file(buffer, read_bytes);
			total_received_bytes += read_bytes;
		}
	}

	std::cout << "Received data size: " << total_received_bytes << " byte(s)." << std::endl;
}
