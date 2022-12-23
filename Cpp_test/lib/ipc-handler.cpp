#include "ipc-handler.h"
#include <ctime>
#include <iostream>

void ipcHandler::select_options(IPCMode mode, int argc, char *argv[])
{
	this->info.mode = mode;
	int opt;
	int fflag = 0;
	int qflag = 0;
	int pflag = 0;
	int sflag = 0;
	int hflag = 0;

	if (argc > 5)
	{
		throw std::runtime_error("ERROR: Too many arguments, use -h for more information.\n");
	}

	while ((opt = getopt(argc, argv, "hf:q:p:s:")) != -1)
	{
		switch (opt)
		{
		case 'h':
			info.protocol = IPCProtocol::HELP;
			hflag = true;
			std::cout << "<HELP>\n"
						 "-q /<queue_name>, use queue as IPC method\n"
						 "-p <pipe_name>, uses pipes as IPC method\n"
						 "-s <shared_name> uses shared memory as IPC method\n"
						 "-f <filename>, file used to read/write data\n"
						 "Method name and file name can not start by '-'.\n"
						 "Example: ./ipc_send -q /queue_method -f file_name\n"
						 "</HELP>\n";
			break;
		case 'q':
			qflag ++;
			info.method_name = optarg;
			break;
		case 'p':
			pflag ++;
			info.method_name = optarg;
			break;
		case 's':
			sflag ++;
			info.method_name = optarg;
			break;
		case 'f':
			fflag ++;
			info.file_name = optarg;
			break;
		default: /* '?' */
			info.protocol = IPCProtocol::NONE;
			throw std::runtime_error("Usage: \n"
									 "-h <--help print out containing description of all supported command-line arguments>\n");
		}
	}

	if (fflag)
	{
		if (pflag + qflag + sflag != 1)
		{
			if (info.file_name[0] == '-')
			{
				throw std::runtime_error("ERROR: File name can not start by '-', use -h for more information.\n");
			}
			else
			{
				std::cout << "name of file is used: " << info.file_name << std::endl;
				std::cout << "Select one IPC method!e.g \n"
							 "./ipcSend -q /<queue_name> -f <file_name>\n"
							 "./ipcSend -p <pipe_name> -f <file_name>\n"
							 "./ipcSend -s <shared_name> -f <file_name>\n";
			}
		}
		else
		{
			if (pflag == 1)
			{
				if (info.method_name[0] == '-')
				{
					throw std::runtime_error("ERROR: Method name can not start by '-', use -h for more information.\n");
				}
				else
				{
					std::cout << "Pipe is used, pipe name is " << info.method_name << std::endl;
					std::cout << "File name: " << info.file_name << std::endl;
					info.protocol = IPCProtocol::PIPE;
				}
			}
			else if (qflag == 1)
			{
				if (info.method_name[0] == '-')
				{
					throw std::runtime_error("ERROR: Method name can not start by '-', use -h for more information.\n");
				}
				else
				{
					std::cout << "queue is used, queue name is " << info.method_name << std::endl;
					std::cout << "File name: " << info.file_name << std::endl;
					info.protocol = IPCProtocol::QUEUE;
				}
			}
			else if (sflag == 1)
			{
				if (info.method_name[0] == '-')
				{
					throw std::runtime_error("ERROR: Method name can not start by '-', use -h for more information.\n");
				}
				else
				{
					std::cout << "shared memory is used, shared name is " << info.method_name << std::endl;
					std::cout << "File name: " << info.file_name << std::endl;
					info.protocol = IPCProtocol::SHARE;
				}
			}
		}
	}
	else if(qflag >= 2 || pflag >= 2 || sflag >= 2 )
	{
		throw std::runtime_error("ERROR: Method must be applied 1 time, use -h for more information.\n");
	}
	else if(qflag + pflag + sflag >= 2)
	{
		throw std::runtime_error("ERROR: Only use 1 method for each time, use -h for more information.\n");
	}
	else if((qflag ==1 &&info.method_name[0] == '-') || (pflag ==1 &&info.method_name[0] == '-') || (sflag ==1 &&info.method_name[0] == '-'))
	{
		throw std::runtime_error("ERROR: No file given and Method name can not start by '-', use -h for more information.\n");
	}
	else if (argc == 1)
	{
		info.protocol = IPCProtocol::NONE;
	}
	else if(hflag == 0)
	{
		throw std::runtime_error("ERROR: No file given, use -h for more information.\n");
	}
	
}

const ipc_info &ipcHandler::get_options() const
{
	return info;
}

IPCProtocol ipcHandler::start() const
{

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
		if (info.protocol == IPCProtocol::NONE)
		{
			throw std::runtime_error("ERROR: missing/wrong arguments, use -h for more information.\n");
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
		if (info.protocol == IPCProtocol::PIPE)
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
		if (info.protocol == IPCProtocol::NONE)
		{
			throw std::runtime_error("ERROR: missing/wrong arguments, use -h for more information.\n");
		}
	}

	return info.protocol;
}
