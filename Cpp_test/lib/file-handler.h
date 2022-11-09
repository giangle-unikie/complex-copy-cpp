#ifndef FILE_HANDLER_H_
#define FILE_HANDLER_H_

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <utility>
#include <vector>


enum class FileMode
{
	NONE, READ, WRITE
};

class FileHandler
{
	protected:
		std::fstream fs{};
		char *file_name;
		FileMode mode{};
	public:
		explicit FileHandler(char *fn, FileMode m) 
			: file_name(fn), mode(m)
		{};
		~FileHandler();
		unsigned long get_file_size();
		void close_file();
		void open_file();
		void read_file(std::vector<char> &data, std::streamsize &data_size);
		void write_file(std::vector<char> &data, std::streamsize &data_size);
		
};


#endif