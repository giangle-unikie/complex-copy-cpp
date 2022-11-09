#include "file-handler.h"
#include <iostream>


FileHandler::~FileHandler()
{
	if (fs.is_open())
		fs.close();
}


void FileHandler::open_file()
{
	if (this->mode == FileMode::READ)
		this->fs.open(this->file_name, std::fstream::in | std::fstream::binary);
	if (this->mode == FileMode::WRITE)
		this->fs.open(this->file_name, std::fstream::out | std::fstream::trunc | std::fstream::binary);
	if (!this->fs)	
	 	throw std::runtime_error("ERROR: open file.");
}

void FileHandler::write_file(std::vector<char> &data, std::streamsize &data_size)
{
	if (this->mode == FileMode::WRITE)
	{
		this->fs.write(data.data(), data_size);
		// check read/writing error on i/o operation
		if (!this->fs.eof() && (this->fs.bad() || this->fs.fail())) {
			throw std::runtime_error("ERROR: istream::read().");
		}
	}
	else
		throw std::runtime_error("ERROR: write_file() in a wrong mode.");
}

void FileHandler::read_file(std::vector<char> &data, std::streamsize &data_size)
{
	if (this->mode == FileMode::READ)
	{
		this->fs.read(data.data(), data_size);
		// check read/writing error on i/o operation
		if (!this->fs.eof() && (this->fs.bad() || this->fs.fail())) {
			throw std::runtime_error("ERROR: istream::read().");
		}
		
	}
	else
		throw std::runtime_error("ERROR: read_file() in a wrong mode.");
}

void FileHandler::close_file()
{
	fs.close();
}

size_t FileHandler::get_file_size()
{
	struct stat st;
	if(this->mode == FileMode::WRITE)
	{
		this->close_file();
	}

    if(stat(this->file_name, &st) < 0) {
		throw std::runtime_error("ERROR: get_file_size().");	
    }

	return st.st_size;

} 