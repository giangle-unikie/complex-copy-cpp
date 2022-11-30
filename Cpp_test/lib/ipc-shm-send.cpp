#include "ipc-shm-send.h"

IPCShmSend::~IPCShmSend()
{
	shm_unlink(this->info.method_name);
}

void IPCShmSend::init()
{
	this->file_handler.open_file();
	this->open_shm();
	this->set_shm_size();
	this->map_shm();
	this->init_mutex();
	this->init_cond();
}

void IPCShmSend::transfer()
{
	long read_bytes{0};
	unsigned long long file_size{this->file_handler.get_file_size()};
	if (file_size == 0)
	{
		throw std::runtime_error("ERROR: File size = 0.");
	}

	unsigned long long total_sent_bytes{0};
	std::cout << "Waiting for receiver..." << std::endl;
	while (!this->is_end)
	{
		lock_mutex();
	
		if (this->shm_ptr->is_read)
		{
			std::cout << "map" << std::endl;
			if (total_sent_bytes < file_size)
			{
				this->file_handler.read_file_shm(&shm_ptr->data_ap[0], this->shm_ptr->data_size);
				read_bytes = this->file_handler.get_read_bytes();
				if (read_bytes > 0)
				{
					this->shm_ptr->is_read = false;
					this->shm_ptr->data_version++;
					this->shm_ptr->data_size = read_bytes;
					total_sent_bytes += read_bytes;
				}
			}
			else
			{
				this->shm_ptr->is_read = false;
				this->shm_ptr->data_size = 0;
				this->shm_ptr->data_version++;
				this->is_end = true;
			}
		}
		unlock_mutex();
		send_cond_broadcast();
	}

	if (total_sent_bytes == file_size && this->is_end)
	{
		std::cout << "Sent: " << total_sent_bytes << " byte(s)." << std::endl;
	}
	else
	{
		throw std::runtime_error("ERROR: The size of Total send file is not equal to File size.");
	}
}

void IPCShmSend::map_shm()
{
	errno = 0;
	this->shm_ptr = (ipc_shm_header_t *)mmap64(NULL, this->shm_size_in_bytes,
											   PROT_READ | PROT_WRITE, MAP_SHARED,
											   this->shmd, 0);
	if (this->shm_ptr == MAP_FAILED)
	{
		throw std::runtime_error("ERROR: mmap64().");
	}

	// set values for new share memory
	this->shm_ptr->is_init = true;
	this->shm_ptr->is_read = true;
	this->shm_ptr->shared_mem_size = this->shm_size_in_bytes;
	this->shm_ptr->data_size = this->shm_size_in_bytes - (std::streamsize)sizeof(ipc_shm_header_t);
}

void IPCShmSend::open_shm()
{
	// remove old shared memory (if any)
	shm_unlink(this->info.method_name);
	errno = 0;
	this->shmd = shm_open(this->info.method_name, O_RDWR | O_CREAT | O_EXCL, 0660);
	if (shmd == -1)
	{
		throw std::runtime_error("ERROR: shm_open() send.");
	}
}
