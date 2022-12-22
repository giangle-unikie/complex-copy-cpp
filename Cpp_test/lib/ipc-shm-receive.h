#ifndef IPC_SHM_RECEIVE_H
#define IPC_SHM_RECEIVE_H

#include "ipc.h"
#include "file-handler.h"
#include "ipc-shm.h"

class IPCShmReceive: public IPC, public IPCShm
{
	protected:
		size_t last_version{0};
        FileHandler file_handler;
	public:
		explicit IPCShmReceive(const ipc_info &info)
		: IPC(info),
			file_handler(info.file_name, FileMode::WRITE){};
		~IPCShmReceive();
		void init() override;
		void map_shm();
		void open_shm();
		void transfer() override;
};


#endif //IPCSHMRECEIVE_H
