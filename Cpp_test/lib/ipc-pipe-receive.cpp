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
	this->pd = open(this->info.method_name, O_RDONLY | O_NONBLOCK);
	if (this->pd == -1){
		throw std::runtime_error(std::string("ERROR: open pipe send name ") + this->info.method_name );
	}
	else{
		std::cout << this->info.method_name << " is opened." << std::endl;	
	}
	this->file_handler.open_file();
}

void IPCPipeReceive::transfer()
{
	long read_bytes{1};
	std::vector<char> buffer(this->p_msgsize);
	std::cout << "Receiving..." << std::endl;
	while (read_bytes != 0)
	{
		read_bytes = read(this->pd, buffer.data(), this->p_msgsize);
		if (read_bytes == 0){
			break;
		}

		if (read_bytes > 0)
		{
			this->file_handler.write_file(buffer, read_bytes);
		}
	}
	std::cout << "Received data size: " << this->file_handler.get_file_size() << " byte(s)" << std::endl;

}
