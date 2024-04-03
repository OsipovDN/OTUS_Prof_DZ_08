#pragma once
#include "FileInfo"



class FileReader
{
private:
	std::streampos _currentPos = 0;			//the current position of the block

	unsigned long long checkSizeBlock(std::ifstream& file);
public:
	FileReader() = default;
	std::string readBlockInFile(bf::path& path);

};