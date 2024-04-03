#pragma once
//STL
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//Boost
#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;
using Data = std::vector<std::string>;

enum class Hash;

class FileReader
{
private:
	unsigned long long _blockSize;		///<block size for comparison
	std::streampos _currentPos;		///<the current position of the block
	char* _buf;							///<block

	unsigned long long checkSizeBlock(std::ifstream& file);
public:
	FileReader(unsigned long long size) :
		_blockSize(size)
	{
		_buf = new char[_blockSize];
	}
	~FileReader() { delete[] _buf; }
	Data readFile(bf::path& path);

};