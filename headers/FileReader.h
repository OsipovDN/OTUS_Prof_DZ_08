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
	/*! The method checks the block size to match the one specified by the user.
		\param file - the file in which the check is performed.
		\return unsigned long long - block size.
	*/
	unsigned long long checkSizeBlock(std::ifstream& file);
public:
	FileReader(unsigned long long size) :
		_blockSize(size),
		_currentPos(0)
	{
		_buf = new char[_blockSize];
	}
	~FileReader() { delete[] _buf; }
	unsigned long long getBlockSize() { return _blockSize; }
	Data readFile(const bf::path& path);

};