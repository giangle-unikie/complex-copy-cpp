#include "ipc-shm.h"

IPCShm::~IPCShm()
{
}

void IPCShm::init_cond()
{

	pthread_condattr_destroy(&(this->cond_attr));
	if (pthread_condattr_init(&(this->cond_attr)) != 0)
	{
		throw std::runtime_error("Error at pthread_condattr_init()");
	}

	if (pthread_condattr_setpshared(&(this->cond_attr), PTHREAD_PROCESS_SHARED) != 0)
	{
		throw std::runtime_error("Error at pthread_condattr_setpshared()");
	}

	if (pthread_cond_init(&(this->shm_ptr->cond), &(this->cond_attr)) != 0)
	{
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_cond_init() send: ") + strerror(errno));
	}

	if (pthread_cond_init(&(this->shm_ptr->cond_re), &(this->cond_attr)) != 0)
	{
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_cond_init() receive: ") + strerror(errno));
	}
}

void IPCShm::init_mutex()
{

	pthread_mutexattr_destroy(&(this->mutex_attr));
	if (pthread_mutexattr_init(&(this->mutex_attr)) != 0)
	{
		throw std::runtime_error("Error at pthread_mutexattr_init()");
	}

	if (pthread_mutexattr_setpshared(&(this->mutex_attr), PTHREAD_PROCESS_SHARED) != 0)
	{
		throw std::runtime_error("Error at pthread_mutexattr_setpshared()");
	}

	if (pthread_mutex_init(&(this->shm_ptr->mutex), &(this->mutex_attr)) != 0)
	{
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_mutex_init(): ") + strerror(errno));
	}
	this->shm_ptr->checkLockMutex = true;
}

void IPCShm::lock_mutex()
{
	if (pthread_mutex_lock(&(this->shm_ptr->mutex)) != 0)
	{
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_mutex_lock(): ") + strerror(errno));
	}
}

void IPCShm::send_cond_broadcast()
{
	if (pthread_cond_broadcast(&(this->shm_ptr->cond)) != 0)
	{
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_cond_broadcast(): ") + strerror(errno));
	}
}

void IPCShm::receive_cond_broadcast()
{
	if (pthread_cond_broadcast(&(this->shm_ptr->cond_re)) != 0)
	{
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_cond_broadcast(): ") + strerror(errno));
	}
}

void IPCShm::set_shm_size() const
{
	if (ftruncate64(this->shmd, this->shm_size_in_bytes) == -1)
	{
		throw std::runtime_error(static_cast<std::string>("ERROR: ftruncate64(): ") + strerror(errno));
	}
}

void IPCShm::unlock_mutex()
{
	if (pthread_mutex_unlock(&(this->shm_ptr->mutex)) != 0)
	{
		throw std::runtime_error(static_cast<std::string>("ERROR: pthread_mutex_unlock(): ") + strerror(errno));
	}
}
