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
DuplicateSearcher::Hash DuplicateSearcher::hashType(std::string& type) noexcept
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
/*
void DuplicateSearcher::scanBlock(std::vector<bf::path>& list_path) {
	std::string temp_hash;
	for (auto& file : list_path) 
	{
		temp_hash = readBlockInFile(file);
		temp_hash = getHash(temp_hash);
	}
	_currentPos += _blockSize;
}
*/

void DuplicateSearcher::checkHashInList(std::string& h, bf::path& path, std::unordered_map<std::string, std::vector<bf::path>>& temp) 
{
	if (temp.find(h) != temp.end())
		temp[h].push_back(path);
	else
		temp.insert(std::pair(h, std::vector{ path }));
}

//���� ���������� � ������ �� ������ �����
void DuplicateSearcher::findConcurrence(std::vector<bf::path>& list_path, std::unordered_map<std::string, std::vector<bf::path>>& l) 
{
	std::string block;
	std::string h;
	for (auto file : list_path) 
	{
		block = readBlockInFile(file);
		h = getHash(block);
		checkHashInList(h, file, l);
	}
}

//������ ������ �� ������ ���������� ������
void DuplicateSearcher::cleanList(std::unordered_map<std::string, std::vector<bf::path>>& l)
{
	for (auto it = l.begin(); it != l.end();)
	{
		if (it->second.size() == 1)
			it = l.erase(it);
		else
			++it;
	}
}

bool DuplicateSearcher::isEnd(std::unordered_map<std::string, std::vector<bf::path>>& files) 
{
	bool flag = true;
	for (auto const& [key, value] : files)
	{
		for (auto i = value.begin(); i != value.end(); ++i) 
		{
			if (static_cast<unsigned long long>(_currentPos) <= bf::file_size(*i)) 
			{
				flag = false;
			}
		}
	}
	return flag;
}

void DuplicateSearcher::searchDuplicate(std::vector < bf::path> conteiner) 
{
	static int s = 0;
	std::unordered_map<std::string, std::vector<bf::path>> temp;
	findConcurrence(conteiner, temp);
	cleanList(temp);
	
	if (isEnd(temp)) 
	{
		for (auto h : temp) 
		{
			_listDuplicate.push_back( h.second );
		}
	}
	
	else  {	
		_currentPos += _blockSize;
		for (auto it : temp) 
		{
			std::cout << ++s << std::endl;
			searchDuplicate(it.second);
			std::cout << --s << std::endl;
		}
		_currentPos -= _blockSize;
	}
	

}


void DuplicateSearcher::print(std::unordered_map<std::string, std::vector<bf::path>>& l) 
{
	for (auto const& [key, value] : l) 
	{
		std::cout << "start temp" << std::endl;
		for (auto i = value.begin(); i != value.end(); ++i) 
		{
			std::cout << *i << std::endl;
		}
		std::cout << "end temp" << std::endl;
	}
}
void DuplicateSearcher::print(std::vector<std::vector<bf::path>>& l)
{
	for (auto const& vec : l) 
	{
		std::cout << "start list" << std::endl;
		for (auto file = vec.begin(); file != vec.end(); ++file) 
		{
			std::cout << *file << std::endl;
		}
		std::cout << "end list" << std::endl;
	}
}

