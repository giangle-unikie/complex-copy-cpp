#include "ipc-shm-send.h"
#include <iostream>
#include <cassert>

IPCShmSend::~IPCShmSend()
{

	if (this->shm_ptr != nullptr)
	{

		int ret2 = pthread_cond_destroy(&(this->shm_ptr->cond_re));
		if (ret2 != 0)
		{
			std::cerr << "Error at pthread_cond_destroy(): receive " << strerror(ret2) << std::endl;
		}
	}
	shm_unlink(this->info.method_name);
}

void IPCShmSend::init()
{
	this->file_handler.open_file();
	this->open_shm();
	this->set_shm_size();
	this->map_shm();
	if (this->shmd > 0)
	{
		close(this->shmd);
	}
	pthread_mutexattr_destroy(&(this->mutex_attr));
	this->init_mutex();

	pthread_condattr_destroy(&(this->cond_attr));
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
	while (!this->shm_ptr->is_end)
	{
		lock_mutex();
		while (this->shm_ptr->data_version_received != this->shm_ptr->data_version)
		{

			if (pthread_cond_wait(&(this->shm_ptr->cond_re), &(this->shm_ptr->mutex)) != 0)
			{
				unlock_mutex();
				throw std::runtime_error(std::string("ERROR: pthread_cond_wait() send: ") + strerror(errno));
			}
		}

		if (total_sent_bytes < file_size)
		{
			this->file_handler.read_file_shm(this->shm_ptr->data_ap, this->shm_ptr->data_size);
			read_bytes = this->file_handler.get_read_bytes();

			if (read_bytes > 0)
			{
				this->shm_ptr->data_version++;
				this->shm_ptr->data_size = read_bytes;
				total_sent_bytes += read_bytes;
			}
		}
		else
		{
			this->shm_ptr->data_size = 0;
			this->shm_ptr->data_version++;
			this->shm_ptr->is_end = true;
		}
		send_cond_broadcast();
		unlock_mutex();
	}

	if (total_sent_bytes == file_size && this->shm_ptr->is_end)
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
	this->shm_ptr = (ipc_shm_header_t *)mmap64(NULL, sizeof(ipc_shm_header_t),
											   PROT_READ | PROT_WRITE, MAP_SHARED,
											   this->shmd, 0);
	if ((static_cast<void *>(this->shm_ptr)) == MAP_FAILED)
	{
		throw std::runtime_error("ERROR: mmap64().");
	}
	this->shm_ptr->data_ap = static_cast<char *>(mmap64(NULL, this->size_of_data,
														PROT_READ | PROT_WRITE, MAP_SHARED,
														this->shmd, 4096));
	if ((static_cast<void *>(this->shm_ptr->data_ap)) == MAP_FAILED)
	{
		throw std::runtime_error("ERROR: mmap64() data_ap.");
	}
	// set values for new share memory
	this->shm_ptr->is_init = true;
	this->shm_ptr->shared_mem_size = this->shm_size_in_bytes;
	this->shm_ptr->data_size = 2048;
}

void IPCShmSend::open_shm()
{
	// remove old shared memory (if any)
	shm_unlink(this->info.method_name);

	this->shmd = shm_open(this->info.method_name, O_RDWR | O_CREAT | O_EXCL, 0660);
	if (shmd == -1)
	{
		throw std::runtime_error("ERROR: shm_open() send.");
	}
}