#ifndef IPC_SHM_H
#define IPC_SHM_H

#include <iostream>
#include <pthread.h>
#include <unistd.h> 
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <vector>

struct ipc_shm_header_t
{
	pthread_cond_t cond;
	pthread_cond_t cond_re;
	pthread_mutex_t mutex;
	std::streamsize data_size{};
	size_t data_version;
	size_t data_version_received;
	std::streamsize shared_mem_size;
	volatile bool is_init {false};
	volatile bool is_read;
	char *data_ap;
	char *data_ap_received;
};

class IPCShm
{
	protected:
		int shmd{};
		pthread_mutexattr_t mutex_attr{};
		pthread_condattr_t cond_attr{};
		ipc_shm_header_t *shm_ptr{};
		std::streamsize shm_size_in_bytes{8192};
		bool is_end{false};
	public:
		explicit IPCShm(){};
		~IPCShm();
		void init_cond();
		void init_mutex();
		void lock_mutex();
		void send_cond_broadcast();
		void receive_cond_broadcast();
		void set_shm_size() const;
		void unlock_mutex();
};


#endif 