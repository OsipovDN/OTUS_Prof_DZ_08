/*! \DuplicateSearch
	\brief source file

	Implementation DuplicateSearch.
*/

///@{
#include "DuplicateSearch.h"

DuplicateSearcher::DuplicateSearcher(unsigned long long blockSize, std::string& hash) :_blockSize(blockSize)
{
	_buf = new char[_blockSize];
	auto myToUpper = [](char ch) {return static_cast<char>(std::toupper(static_cast<unsigned char>(ch))); };
	std::transform(hash.begin(), hash.end(), hash.begin(), myToUpper);
	_hash = hashType(hash);
}
DuplicateSearcher::~DuplicateSearcher()
{
	delete[] _buf;
}
std::string DuplicateSearcher::getHash(std::string& block)
{
	std::string hash_transf;
	switch (_hash)
	{
	case Hash::CRC32:
		hash_transf = CRC32_function((unsigned char*)block.c_str(), _blockSize);
		break;
	case Hash::MD5:
		hash_transf = md5(block);
		break;
	case Hash::NONE:
		break;
	default:
		break;
	}
	return hash_transf;
}
Hash DuplicateSearcher::hashType(std::string& type) noexcept
{
	if (type == "MD5")
		return Hash::MD5;
	else if (type == "CRC32")
		return Hash::CRC32;
	else
		return Hash::NONE;
}
unsigned long long DuplicateSearcher::checkSizeBlock(std::ifstream& file)
{
	file.seekg(_currentPos, std::ios_base::beg);
	unsigned long long start = file.tellg();
	file.seekg(0, std::ios_base::end);
	unsigned long long end = file.tellg();
	auto remains = end - start;
	return  remains;
}

std::string DuplicateSearcher::readBlockInFile(bf::path& path)
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

void DuplicateSearcher::checkHashInList(std::string& hash, bf::path& file)
{

	if (_currentBlock.find(hash) != _currentBlock.end())
		_currentBlock[hash].push_back(file);
	else
		_currentBlock.insert(std::pair(hash, std::vector{ file }));

}

void DuplicateSearcher::findConcurrence(std::vector<bf::path>& listFile)
{
	std::string block;
	std::string hash;
	for (auto file : listFile)
	{
		block = readBlockInFile(file);
		hash = getHash(block);
		checkHashInList(hash, file);
	}
}

void DuplicateSearcher::removeUniqHash()
{
	for (auto it = _currentBlock.begin(); it != _currentBlock.end();)
	{
		if (it->second.size() == 1)
			it = _currentBlock.erase(it);
		else
			++it;
	}
}

bool DuplicateSearcher::isEnd()
{
	for (auto const& [hash, files] : _currentBlock)
	{
		for (auto file = files.begin(); file != files.end(); ++file)
		{
			if (static_cast<unsigned long long>(_currentPos) <= bf::file_size(*file))
			{
				return false;
			}
		}
	}
	return true;
}

void DuplicateSearcher::searchDuplicate(std::vector < bf::path>& conteiner)
{
	_currentBlock.clear();
	static int recursionDepth = 0;
	findConcurrence(conteiner);
	removeUniqHash();
	if (isEnd())
	{
		for (auto h : _currentBlock)
		{
			_listDuplicate.push_back(h.second);
		}
	}
	else
	{
		_currentPos += _blockSize;
		_stack.push(_currentBlock);
		for (auto it : _stack.top())
		{
			std::cout << ++recursionDepth << std::endl;
			searchDuplicate(it.second);
			std::cout << --recursionDepth << std::endl;
		}
		_stack.pop();
		_currentPos -= _blockSize;
	}
}

void DuplicateSearcher::print(HashNode& listCurrentHash)
{
	for (auto const& [hash, filelist] : listCurrentHash)
	{
		std::cout << "Matches the hash: " << hash << std::endl;
		std::for_each(filelist.rbegin(), filelist.rend(), [](const bf::path& file) { std::cout << file.filename().string() << std::endl; });
		std::cout << std::endl;
	}
}
void DuplicateSearcher::print()
{
	for (auto const& filelist : _listDuplicate)
	{
		std::for_each(filelist.rbegin(), filelist.rend(), [](const bf::path& file) { std::cout << file.filename().string() << std::endl; });
		std::cout << std::endl;
	}
}
///@}

