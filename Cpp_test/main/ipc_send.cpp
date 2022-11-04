#include <iostream>
#include <string>
#include "lib/ipc-handler.h"
int main(int argc, char *argv[])
{
	try{
		ipcHandler ipc;
		ipc.select_options(IPCMode::SEND_MODE,argc, argv);
		ipc.start();
	}
	catch (const std::exception &e)
	{
		std::rethrow_exception(std::current_exception());
	}

	return 0;
}