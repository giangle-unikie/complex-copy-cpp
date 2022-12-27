#include "ipc-handler.h"
#include <ctime>
#include <iostream>

void ipcHandler::select_options(IPCMode mode, int argc, char *argv[])
{
	this->info.mode = mode;
	int opt;
	int file_flag = 0;
	int queue_flag = 0;
	int pipe_flag = 0;
	int shared_flag = 0;
	int help_flag = 0;

	struct option long_options[] =
	{	
		{"help", no_argument, NULL, 'h'},
		{"queue", required_argument, NULL, 'q'},
		{"pipe", required_argument, NULL, 'p'},
		{"shm", required_argument, NULL, 's'},
		{"file", required_argument, NULL, 'f'},
		{0, 0, NULL, 0}
	};

	if (argc > 5)
	{
		throw std::runtime_error("ERROR: Too much arguments, use --h for more information.\n");
	}
	if (argc == 1)
	{
		throw std::runtime_error("ERROR: missing arguments, use --h for more information.\n");
	}
	if(argc == 2 && argv[1][0] != '-')	// for case: ./ipc_send exam
	{
		throw std::runtime_error("ERROR: Wrong arguments, use --h for more information.\n");
	}

	while ((opt = getopt_long(argc, argv, "hm:p:q:s:f:", long_options, NULL)) != -1)
	{
		switch (opt)
		{
		case 'h':
			info.protocol = IPCProtocol::HELP;
			help_flag = 1;
			std::cout << "<HELP>\n"
						 "--q or --queue /<queue_name>, use queue as IPC method\n"
						 "--p or --pipe <pipe_name>, uses pipes as IPC method\n"
						 "--s or --shm <shared_name> uses shared memory as IPC method\n"
						 "--f --file <filename>, file used to read/write data\n"
						 "Method name and file name can not start by '-'.\n"
						 "Example: ./ipc_send -q /queue_method -f file_name\n"
						 "</HELP>\n";
			break;
		case 'q':
			queue_flag++;
			info.method_name = optarg;
			break;
		case 'p':
			pipe_flag++;
			info.method_name = optarg;
			break;
		case 's':
			shared_flag++;
			info.method_name = optarg;
			break;
		case 'f':
			file_flag++;
			info.file_name = optarg;
			break;
		default: /* '?' */
			info.protocol = IPCProtocol::NONE;
			throw std::runtime_error("Usage: \n"
									 "-h <--help print out containing description of all supported command-line arguments>\n");
		}
	}

	if (file_flag)
	{
		if (info.file_name[0] == '-')
		{
			throw std::runtime_error("ERROR: example file name: file_name. File name can not start by '-', use --h for more information.\n");
		}
		if (pipe_flag + queue_flag + shared_flag != 1)
		{

			std::cout << "name of file is used: " << info.file_name << std::endl;
			std::cout << "Select one IPC method!e.g \n"
						 "./ipc_send --q /<queue_name> --f <file_name>\n"
						 "./ipc_send --p <pipe_name> --f <file_name>\n"
						 "./ipc_send --s <shared_name> --f <file_name>\n";
		}
		else if (pipe_flag + queue_flag + shared_flag == 1)
		{
			if (info.method_name[0] == '-')
			{
				throw std::runtime_error("ERROR: example method name: pipe_name. Method name can not start by '-', use --h for more information.\n");
			}
			if (pipe_flag == 1)
			{

				
					std::cout << "Pipe is used, pipe name is " << info.method_name << std::endl;
					std::cout << "File name: " << info.file_name << std::endl;
					info.protocol = IPCProtocol::PIPE;
				
			}
			else if (queue_flag == 1)
			{
				
					std::cout << "queue is used, queue name is " << info.method_name << std::endl;
					std::cout << "File name: " << info.file_name << std::endl;
					info.protocol = IPCProtocol::QUEUE;
				
			}
			else if (shared_flag == 1)
			{
				
					std::cout << "shared memory is used, shared name is " << info.method_name << std::endl;
					std::cout << "File name: " << info.file_name << std::endl;
					info.protocol = IPCProtocol::SHARE;
				
			}
		}
	}
	else if (queue_flag >= 2 || pipe_flag >= 2 || shared_flag >= 2)
	{
		throw std::runtime_error("ERROR: Method must be applied 1 time, use --h for more information.\n");
	}
	else if (queue_flag + pipe_flag + shared_flag >= 2)
	{
		throw std::runtime_error("ERROR: Only use 1 method for each time, use --h for more information.\n");
	}
	else if (help_flag == 0)
	{
		throw std::runtime_error("ERROR: No file given, use --h for more information.\n");
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
			throw std::runtime_error("ERROR: missing/wrong arguments, use --h for more information.\n");
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
			throw std::runtime_error("ERROR: missing/wrong arguments, use --h for more information.\n");
		}
	}

	return info.protocol;
}
