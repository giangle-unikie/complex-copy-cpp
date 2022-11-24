#include "ipc-pipe-send.h"

IPCPipeSend::~IPCPipeSend()
{
	close(this->pd);
	unlink(this->info.method_name);
}

void IPCPipeSend::init()
{
	this->file_handler.setup_file(info.file_name, FileMode::READ);
	// remove old pipe (if any)
	bool remove_old_pipe = remove(this->info.method_name);
	
	if(!remove_old_pipe){
		throw std::runtime_error("ERROR: Fail to remove old pipe.");
	}
	// create a fifo
	if (mkfifo(this->info.method_name, 0660) != 0){
        throw std::runtime_error(std::string("ERROR: create pipe with send name ") + this->info.method_name );
    }
		
	// open the pipe
	this->pd = open(this->info.method_name, O_RDWR | O_NONBLOCK);
	if (this->pd == -1){
		throw std::runtime_error(std::string("ERROR: open pipe send name ") + this->info.method_name );
	}
	else{
		std::cout << "[" << this->info.method_name << "] is opened." << std::endl;
	}
	this->file_handler.open_file();
}

void IPCPipeSend::transfer()
{
	std::vector<char> buffer(this->p_msgsize);
	long sent_bytes{0};
	long read_bytes{0};
	unsigned long long total_sent_bytes{0};
	unsigned long long file_size = this->file_handler.get_file_size();
	if (file_size == 0){
        throw std::runtime_error("ERROR: File size = 0.");
    }
		
	std::cout << "Sending..." << std::endl;
    sleep(3);
	while (total_sent_bytes < file_size)
	{
		this->file_handler.read_file(buffer, this->p_msgsize);
		
		read_bytes = this->file_handler.get_read_bytes();
		
		if (read_bytes > 0)
		{
			sent_bytes = write(this->pd, buffer.data(), read_bytes);
           
			if (sent_bytes == read_bytes)
			{
				total_sent_bytes += sent_bytes;
			}
			else{
                throw std::runtime_error(std::string("ERROR: write() while send in pipe"));
            }
				
		} else if (read_bytes == 0){
			break;
		}
	}

	if (total_sent_bytes == file_size){
		std::cout << "Sent data size: " << total_sent_bytes << " byte(s)." << std::endl;
	}
	else{
		throw std::runtime_error("ERROR: Uncompleted transfer");
	}
}