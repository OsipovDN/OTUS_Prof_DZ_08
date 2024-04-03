#include "FileReader.h"

unsigned long long FileReader::checkSizeBlock(std::ifstream& file)
{
	file.seekg(_currentPos, std::ios_base::beg);
	unsigned long long start = file.tellg();
	file.seekg(0, std::ios_base::end);
	unsigned long long end = file.tellg();
	auto remains = end - start;
	return  remains;
}

std::string FileReader::readBlockInFile(bf::path& path)
{
	std::ifstream file;
	file.open(path.string(), std::ios_base::binary);
	if (!file.is_open())
	{
		std::cout << "File is not open!" << std::endl;
	}

	auto count_byte = checkSizeBlock(file);
	if (count_byte < _blockSize || count_byte == 0)
	{
		for (auto i = count_byte; i != _blockSize; ++i)
			_buf[i] = '\0';
	}

	file.seekg(_currentPos, std::ios_base::beg);
	file.read(_buf, _blockSize);
	file.close();
	return std::string{ _buf };
}