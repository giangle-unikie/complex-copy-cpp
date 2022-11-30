#include "ipc-handler.h"
#include <ctime>
#include <iostream>


void ipcHandler::select_options(IPCMode mode, int argc, char *argv[]){
	this->info.mode = mode;
  	int opt;
	bool fflag = false;
	bool qflag = false;
	bool pflag = false;
	bool sflag = false;
  	bool hflag = false;


	while ((opt = getopt(argc, argv, "hf:q:p:s:")) != -1) {
		switch (opt) {
			case 'h':
				info.protocol = IPCProtocol::HELP;	
        		hflag = true;
				std::cout << "<HELP>\n"
					"-q /<queue_name>, use queue as IPC method\n"
					"-p <pipe_name>, uses pipes as IPC method\n"
					"-s <buffer size in kB>, uses shared memory as IPC method\n"
					"-f <filename>, file used to read/write data\n"
					"</HELP>\n";
			break;
			case 'q':
				qflag = true; 
				info.method_name = optarg;
				break;
			case 'p':
				pflag = true;
				info.method_name = optarg;
				break;
			case 's':
				sflag = true;
				info.method_name = optarg;
				break;
			case 'f':
				fflag = true;
				info.file_name = optarg;				
				break;
			default: /* '?' */
				info.protocol = IPCProtocol::NONE;
				throw std::runtime_error("Usage: \n"
						"-h <--help print out containing description of all supported command-line arguments>\n");
		}
	}
  if (fflag) {
		if ( !pflag && !qflag && !sflag) {
			std::cout << "name of file is used: " << info.file_name << std::endl;
			std::cout << "Select one IPC method!e.g \n"
					"./ipcSend -q /<queue_name> -f <file_name>\n"
					"./ipcSend -p <pipe_name> -f <file_name>\n"
					"./ipcSend -s <shared_name> -f <file_name>\n";

		} else {
			if(pflag){
				std::cout << "Pipe is used, pipe name is "<< info.method_name << std::endl;
				std::cout << "File name: " << info.file_name << std::endl;
				info.protocol = IPCProtocol::PIPE;	

			} else if (qflag) {				
				std::cout << "queue is used, queue name is "<< info.method_name << std::endl;
				std::cout << "File name: " << info.file_name << std::endl;
				info.protocol = IPCProtocol::QUEUE;

			} else {
				std::cout << "shared memory is used, shared name is "<< info.method_name << std::endl;
				std::cout << "File name: " << info.file_name << std::endl;
        		info.protocol = IPCProtocol::SHARE;

			}
		}	

	}else if (!hflag && (!pflag && !qflag && !sflag && !fflag)) {
		info.protocol = IPCProtocol::NONE;
		throw std::runtime_error("ERROR: No file given, use -h for more information.");
	}  	
}

const ipc_info &ipcHandler::get_options() const
{
	return info;
}

IPCProtocol ipcHandler::start() const{

	if (info.mode == IPCMode::RECEIVE_MODE)
	{
		if (info.protocol == IPCProtocol::QUEUE)
		{
			ipcQueueReceive mq(this->info);
			mq.init();
			mq.transfer();
		}
		if (info.protocol == IPCProtocol::PIPE)
		{
			IPCPipeReceive pipe(this->info);
			pipe.init();
			pipe.transfer();

		}
		if (info.protocol == IPCProtocol::SHARE)
		{
			IPCShmReceive shm(this->info);
			shm.init();
			shm.transfer();

		}
		if (info.protocol == IPCProtocol::NONE){
			throw std::runtime_error("ERROR: missing/wrong arguments");
		}
	}
	else if (info.mode == IPCMode::SEND_MODE)
	{
		if (info.protocol == IPCProtocol::QUEUE)
		{
			ipcQueueSend mq(this->info);
			mq.init();
			mq.transfer();
		}
		if (info.protocol == IPCProtocol::PIPE )
		{
			IPCPipeSend pipe(this->info);
			pipe.init();
			pipe.transfer();

		}
		if (info.protocol == IPCProtocol::SHARE)
		{
			IPCShmSend shm(this->info);
			shm.init();
			shm.transfer();

		}
		if (info.protocol == IPCProtocol::NONE){
			throw std::runtime_error("ERROR: missing/wrong arguments");
		}
	}
	return info.protocol;
}

