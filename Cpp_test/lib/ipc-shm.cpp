#include "ipc-shm.h"

IPCShm::~IPCShm()
{
	munmap(this->shm_ptr, this->shm_size_in_bytes);
	if (this->shmd > 0){
		close(this->shmd);
	}
}

void IPCShm::init_cond()
{
	pthread_condattr_init(&(this->cond_attr));
	pthread_condattr_setpshared(&(this->cond_attr), PTHREAD_PROCESS_SHARED);
	if (pthread_cond_init(&(this->shm_ptr->cond), &(this->cond_attr)) != 0){
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_cond_init(): ") + strerror(errno));
	}
		
}

void IPCShm::init_mutex()
{
	pthread_mutexattr_init(&(this->mutex_attr));
	pthread_mutexattr_setpshared(&(this->mutex_attr), PTHREAD_PROCESS_SHARED);
	if (pthread_mutex_init(&(this->shm_ptr->mutex), &(this->mutex_attr)) != 0){
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_mutex_init(): ") + strerror(errno));
	}
		
}

void IPCShm::lock_mutex()
{
	if (pthread_mutex_lock(&(this->shm_ptr->mutex)) != 0){
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_mutex_lock(): ") + strerror(errno));
	}	
}


void IPCShm::send_cond_broadcast()
{
	if (pthread_cond_broadcast(&(this->shm_ptr->cond)) != 0){
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_cond_broadcast(): ") + strerror(errno));
	}	
}

void IPCShm::set_shm_size() const
{
	if (ftruncate64(this->shmd, this->shm_size_in_bytes) == -1){
		throw std::runtime_error(static_cast<std::string>("ERROR: ftruncate64(): ") + strerror(errno));
	}
}

void IPCShm::unlock_mutex()
{
	if (pthread_mutex_unlock(&(this->shm_ptr->mutex)) != 0){
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_mutex_unlock(): ") + strerror(errno));
	}
}