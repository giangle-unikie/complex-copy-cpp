#include "ipc-pipe-receive.h"

IPCPipeReceive::~IPCPipeReceive()
{
	close(this->pd);
	unlink(this->info.method_name);
	// Release semaphore
	sem_post(this->sem);
	sem_close(this->sem);
	// Remove semaphores
	sem_unlink(this->sem_name.c_str());
}

void IPCPipeReceive::init()
{
	this->file_handler.setup_file(info.file_name, FileMode::WRITE);
	std::cout << "Waiting for sender... " << std::endl;
	int time_wait{0};
	std::string name;

	for (time_wait = 0; time_wait < 10; time_wait++)
	{
		std::cout << time_wait << std::endl;

		if ((this->pd = open(this->info.method_name, O_RDONLY)) != -1)
		{

			break;
		}
		sleep(1);
	}
	if (time_wait >= 10)
	{
		throw std::runtime_error("ERROR: Running out of time wait of receiver, 10s \n");
	}

	std::cout << this->info.method_name << " is opened." << std::endl;

	this->file_handler.open_file();
	/* There might be multiple receivers. We must ensure that
			only one receiver uses sem at a time.  */
	// Open semaphore
	name = '/';
	this->sem_name = name + this->info.method_name;

	this->sem = sem_open(this->sem_name.c_str(), O_CREAT , 0660, 1);
	
	if (this->sem == SEM_FAILED)
	{
		throw std::runtime_error(std::string("ERROR: Fail to open semaphore: ") + strerror(errno));
	}
}

void IPCPipeReceive::transfer()
{
	long read_bytes{1};
	unsigned long long total_received_bytes{0};
	std::vector<char> buffer(this->p_msgsize);
	std::cout << "Receiving..." << std::endl;

	// takes control, semaphore locked
	int t = sem_wait(this->sem);
	if (t == -1)
	{
		throw std::runtime_error(std::string("ERROR: Fail sem_wait: ") + strerror(errno));
	}

	while (true)
	{

		read_bytes = read(this->pd, buffer.data(), this->p_msgsize);
		if (read_bytes == 0)
		{
			break;
		}

		if (read_bytes > 0)
		{
			this->file_handler.write_file(buffer, read_bytes);
			total_received_bytes += read_bytes;
		}
	}

	std::cout << "Received data size: " << total_received_bytes << " byte(s)." << std::endl;
	std::cout << "File size: " << this->file_handler.get_file_size() << " byte(s)." << std::endl;
}
