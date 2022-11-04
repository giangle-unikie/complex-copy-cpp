#include "lib/file-handler.h"
#include "gtest/gtest.h"


TEST(FileTestCases, openFileNotExist){
	char file_name[] = "FileNotExist";

	FileHandler openFileNotExist(file_name, FileMode::READ);
	EXPECT_THROW(openFileNotExist.open_file(), std::runtime_error);
}


TEST(FileTestCases, readFile){
    std::streamsize stringSize{8};
	char file_name[] = "testFile/test";

	std::vector<char> readString{};
	readString.resize(stringSize);

	FileHandler fileRead(file_name, FileMode::READ);
	fileRead.open_file();
	EXPECT_NO_THROW(fileRead.read_file(readString, stringSize));
	EXPECT_THROW(fileRead.write_file(readString, stringSize), std::runtime_error);
}

TEST(FileTestCases, writeFile){
    std::streamsize stringSize{8};
	char file_name[] = "writeTest";

	std::vector<char> writeString{'T','i','m','e','l','i','n','e'};
	writeString.resize(stringSize);

	FileHandler fileWrite(file_name, FileMode::WRITE);
	fileWrite.open_file();
	EXPECT_NO_THROW(fileWrite.write_file(writeString, stringSize));
	EXPECT_EQ(fileWrite.get_file_size(), stringSize);
}