#include "ipc-shm-receive.h"

IPCShmReceive::~IPCShmReceive()
{
	shm_unlink(this->info.method_name);
}

void IPCShmReceive::init()
{
	this->file_handler.open_file();
	this->open_shm();
	this->map_shm();
}

void IPCShmReceive::transfer()
{
	std::cout << "Receiving..." << std::endl;
	unsigned long total_received_bytes{0};

	while (!this->is_end && this->shm_ptr->is_init)
	{
		lock_mutex();
		while (this->last_version == this->shm_ptr->data_version)
		{
			int i = 0;
			i++;
			if (i == 10)
			{
				throw std::runtime_error("ERROR: data version does not change.");
			}

			if (pthread_cond_wait(&(this->shm_ptr->cond), &(this->shm_ptr->mutex)) != 0)
			{
				throw std::runtime_error(std::string("ERROR: pthread_cond_wait(): ") + strerror(errno));
			}
		}
		if (this->shm_ptr->shared_mem_size != this->shm_size_in_bytes)
		{
			throw std::runtime_error("ERROR: Shared memory size of server and client side are not the same.");
		}

		if (this->shm_ptr->data_size != 0)
		{
			this->file_handler.write_file_shm(&this->shm_ptr->data_ap[0], this->shm_ptr->data_size);

			total_received_bytes += this->shm_ptr->data_size;
		}
		else
		{
			this->is_end = true;
		}

		this->shm_ptr->is_read = true;
		this->last_version = this->shm_ptr->data_version;
		unlock_mutex();
	}

	std::cout << "Received data size: " << total_received_bytes << " byte(s)" << std::endl;
}

void IPCShmReceive::map_shm()
{
	for (unsigned tries = 0;;)
	{
		this->shm_ptr = (ipc_shm_header_t *)mmap64(NULL, this->shm_size_in_bytes,
												   PROT_READ | PROT_WRITE, MAP_SHARED,
												   this->shmd, 0);
		if (this->shm_ptr != MAP_FAILED)
		{
			break;
		}

		++tries;
		sleep(1);
	}
}

void IPCShmReceive::open_shm()
{
	std::cout << "Waiting for sender..." << std::endl;
	for (unsigned tries = 0;;)
	{
		this->shmd = shm_open(this->info.method_name, O_RDWR, 0660);
		if (this->shmd != -1)
		{
			break;
		}
		++tries;
		sleep(1);
	}
}
