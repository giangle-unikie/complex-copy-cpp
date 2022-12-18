#include "ipc-shm-receive.h"

IPCShmReceive::~IPCShmReceive()
{

	shm_unlink(this->info.method_name);
	if (pthread_cond_destroy(&(this->shm_ptr->cond_re)) != 0)
	{
		std::cerr << "Error at pthread_cond_destroy(): receive" << std::endl;
	}
	if (pthread_cond_destroy(&(this->shm_ptr->cond)) != 0)
	{
		std::cerr << "Error at pthread_cond_destroy(): send" << std::endl;
	}
	if (pthread_mutex_destroy(&(this->shm_ptr->mutex)) != 0)
	{
		std::cerr << "Error at pthread_mutex_destroy()" << std::endl;
	}
}

void IPCShmReceive::init()
{
	this->file_handler.open_file();
	this->open_shm();
	this->map_shm();
	if (this->shmd > 0)
	{
		close(this->shmd);
	}
}

void IPCShmReceive::transfer()
{
	std::cout << "Receiving..." << std::endl;
	unsigned long total_received_bytes{0};

	while (!this->is_end && this->shm_ptr->is_init)
	{
		lock_mutex();
		while (this->shm_ptr->data_version_received == this->shm_ptr->data_version)
		{
			if (pthread_cond_wait(&(this->shm_ptr->cond), &(this->shm_ptr->mutex)) != 0)
			{
				throw std::runtime_error(std::string("ERROR: pthread_cond_wait() send: ") + strerror(errno));
			}
		}

		if (this->shm_ptr->shared_mem_size != this->shm_size_in_bytes)
		{
			pthread_mutex_destroy(&(this->shm_ptr->mutex));
			throw std::runtime_error("ERROR: Shared memory size of server and client side are not the same.");
		}

		if (this->shm_ptr->data_size != 0)
		{
			this->file_handler.write_file_shm(this->shm_ptr->data_ap_received, this->shm_ptr->data_size);

			total_received_bytes += this->shm_ptr->data_size;
			this->shm_ptr->data_version_received++;
		}
		else
		{
			this->is_end = true;
			this->shm_ptr->data_version_received++;
		}

		this->shm_ptr->data_version_received = this->shm_ptr->data_version;
		receive_cond_broadcast();
		unlock_mutex();
	}

	std::cout << "Received data size: " << total_received_bytes << " byte(s)" << std::endl;
}

void IPCShmReceive::map_shm()
{
	this->shm_ptr = (ipc_shm_header_t *)mmap64(NULL, sizeof(ipc_shm_header_t),
											   PROT_READ | PROT_WRITE, MAP_SHARED,
											   this->shmd, 0);
	if ((static_cast<void *>(this->shm_ptr)) == MAP_FAILED)
	{
		throw std::runtime_error("ERROR: mmap64().");
	}
	this->shm_ptr->data_ap_received = (char *)mmap64(NULL, this->size_of_data,
													 PROT_READ | PROT_WRITE, MAP_SHARED,
													 this->shmd, 4096);
	if ((static_cast<void *>(this->shm_ptr->data_ap_received)) == MAP_FAILED)
	{
		throw std::runtime_error("ERROR: mmap64() data_ap received.");
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
		if (tries == 10)
		{
			throw std::runtime_error("ERROR: open_shm(), wait 10 second.");
		}
	}
}
