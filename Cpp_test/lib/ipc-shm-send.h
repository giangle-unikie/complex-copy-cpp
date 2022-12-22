#ifndef IPC_SHM_SEND_H
#define IPC_SHM_SEND_H

#include "ipc.h"
#include "file-handler.h"
#include "ipc-shm.h"

class IPCShmSend: public IPC, public IPCShm
{
	protected:
        FileHandler file_handler;

	public:
		explicit IPCShmSend(const ipc_info &info): IPC(info),
                file_handler(info.file_name, FileMode::READ){};

		~IPCShmSend();
		void init() override;
		void transfer() override;
		void open_shm();
		void map_shm();
};

#endif 