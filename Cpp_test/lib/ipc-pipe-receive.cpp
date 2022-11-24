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

	
	do
	{
		this->pd = open(this->info.method_name, O_RDONLY );
		sleep(1);
		int i = 0;
		i++;
		if (i == 5)
		{
			throw std::runtime_error("ERROR: Fail to open receive pipe.");
		}
	} while (this->pd == -1);
	
	std::cout << this->info.method_name << " is opened." << std::endl;	
	
	this->file_handler.open_file();
}

void IPCPipeReceive::transfer()
{
	long read_bytes{1};
	unsigned long long total_received_bytes{0};
	std::vector<char> buffer(this->p_msgsize);
	std::cout << "Receiving..." << std::endl;
	while (1)
	{
		read_bytes = read(this->pd, buffer.data(), this->p_msgsize);
		if (read_bytes == 0){
			break;
		}

		if (read_bytes > 0)
		{
			this->file_handler.write_file(buffer, read_bytes);
			total_received_bytes += read_bytes;
		}
	}

	std::cout << "Received data size: "  << total_received_bytes << " byte(s)." << std::endl;

}
