#include "ipc-shm-receive.h"

IPCShmReceive::~IPCShmReceive()
{

	if (this->shm_ptr != nullptr)
	{
		if (this->shm_ptr->checkLockMutex == true)
		{
			this->shm_ptr->checkLockMutex = false;
		}
		int ret1 = pthread_cond_destroy(&(this->shm_ptr->cond));
		if (ret1 != 0)
		{
			std::cerr << "Error at pthread_cond_destroy(): " << strerror(ret1) << std::endl;
		}

		int ret2 = pthread_mutex_trylock(&(this->shm_ptr->mutex));
		if (ret2 != 0)
		{
			std::cerr << "Error at pthread_mutex_trylock(): " << strerror(ret2) << std::endl;
		}
		else
		{
			int ret3 = pthread_mutex_unlock(&(this->shm_ptr->mutex));
			if (ret3 != 0)
			{
				std::cerr << "Error at pthread_mutex_unlock(): " << strerror(ret3) << std::endl;
			}
			else
			{
				int ret4 = pthread_mutex_destroy(&(this->shm_ptr->mutex));
				if (ret4 != 0)
				{
					std::cerr << "Error at pthread_mutex_destroy(): " << strerror(ret4) << std::endl;
				}
			}
		}

		int ret5 = pthread_mutexattr_destroy(&(this->mutex_attr));
		if (ret5 != 0)
		{
			std::cerr << "Error at pthread_mutexattr_destroy(): " << strerror(ret5) << std::endl;
		}
	}
	shm_unlink(this->info.method_name);
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

	while (!this->shm_ptr->is_end && this->shm_ptr->is_init)
	{
		if (this->shm_ptr->checkLockMutex == true)
		{
			lock_mutex();
		}
		while (this->shm_ptr->data_version_received == this->shm_ptr->data_version)
		{
			if (pthread_cond_wait(&(this->shm_ptr->cond), &(this->shm_ptr->mutex)) != 0)
			{
				throw std::runtime_error("ERROR: pthread_cond_wait() send.");
			}
		}

		if (this->shm_ptr->shared_mem_size != this->shm_size_in_bytes)
		{
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
	this->shm_ptr->data_ap_received = static_cast<char *>(mmap(NULL, this->size_of_data,
															   PROT_READ | PROT_WRITE, MAP_SHARED,
															   this->shmd, 4096));
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
		std::cout << tries << std::endl;
		if (tries >= 10)
		{
			throw std::runtime_error("ERROR: open_shm(), wait over 10 second.");
		}
	}
}
