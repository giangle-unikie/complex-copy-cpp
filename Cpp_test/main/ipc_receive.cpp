#include <iostream>
#include <string>
#include "lib/ipc-handler.h"

int main(int argc, char *argv[])
{
	try{
		ipcHandler ipc;
		ipc.select_options(IPCMode::RECEIVE_MODE,argc, argv);
		ipc.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what();
		return -1;
	}

	return 0;
}
