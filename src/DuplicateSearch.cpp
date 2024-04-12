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
	{
		CRC32 crc;
		hash_transf = crc((const char*)block.c_str(), _blockSize);
		break;
	}
	case Hash::MD5:
	{
		MD5 md;
		hash_transf = md((const char*)block.c_str(), _blockSize);
		break;
	}
	case Hash::SHA1:
	{
		SHA1 sha;
		hash_transf = sha((const char*)block.c_str(), _blockSize);
		break;
	}
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

std::string DuplicateSearcher::readBlockInFile(bf::path& path)
{
	std::ifstream file;
	file.open(path.string(), std::ios_base::binary);
	if (!file.is_open())
	{
		std::cout << "File is not open!" << std::endl;
		return std::string{};
	}
	else
	{
		std::streampos pos = _currentPos;
		memset(reinterpret_cast<void*>(_buf), '\0', _blockSize);

		file.seekg(pos, std::ios_base::beg);
		file.read(_buf, _blockSize);
		file.close();
		return std::string{ _buf };
	}
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
			if (_currentPos <= bf::file_size(*file))
			{
				return false;
			}
		}
	}
	return true;
}

void DuplicateSearcher::pushInStack()
{
	std::for_each(_currentBlock.begin(), _currentBlock.end(), [this](std::pair < std::string, std::vector<bf::path>> n)
		{
			_stack.emplace(std::pair< unsigned long long, std::pair < std::string, std::vector<bf::path>> >(_currentPos, n));
		});
}
void getCurrent()
{
	Node tempNode;

}

void DuplicateSearcher::searchDuplicate(std::vector < bf::path>& conteiner)
{
	Node tempNode;
	findConcurrence(conteiner);
	removeUniqHash();
	pushInStack();
	while (!_stack.empty())
	{
		_currentBlock.clear();
		tempNode = _stack.top();
		_stack.pop();
		_currentPos = tempNode.first + _blockSize;
		findConcurrence(tempNode.second.second);
		removeUniqHash();
		pushInStack();

		if (isEnd())
		{
			for (auto h : _currentBlock)
			{
				_listDuplicate.push_back(h.second);
			}
			if (!_stack.empty())
				_stack.pop();
			_currentBlock.clear();
		}
	}
}

void DuplicateSearcher::print(HashMap& listCurrentHash)
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

