#include "DuplicateSearch.h"

DuplicateSearcher::DuplicateSearcher(std::unique_ptr<FileReader> reader, std::string& hash)
{
	_reader = std::move(reader);
	_blockSize = _reader->getBlockSize();
	auto myToUpper = [](char ch) {return static_cast<char>(std::toupper(static_cast<unsigned char>(ch))); };
	std::transform(hash.begin(), hash.end(), hash.begin(), myToUpper);
	_hash = hashType(hash);
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
//unsigned long long DuplicateSearcher::checkSizeBlock(std::ifstream& file) 
//{
//	file.seekg(_currentPos, std::ios_base::beg);
//	unsigned long long start = file.tellg();
//	file.seekg(0, std::ios_base::end);
//	unsigned long long end = file.tellg();
//	auto remains = end - start;
//	return  remains;
//}

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

void DuplicateSearcher::checkHashInList(std::string& hash, bf::path& file, HashFiles& listCurrentHash)
{
	if (listCurrentHash.find(hash) != listCurrentHash.end())
		listCurrentHash[hash].push_back(file);
	else
		listCurrentHash.insert(std::pair(hash, std::vector{ file }));
}

//»щем совпадени€ в списке по одному блоку
void DuplicateSearcher::findConcurrence(std::vector<bf::path>& filelist, HashFiles& listCurrentHash)
{
	std::string block;
	std::string hash;
	for (auto file : filelist)
	{
		block = readBlockInFile(file);
		hash = getHash(block);
		checkHashInList(hash, file, listCurrentHash);
	}
}

//„мстим список от лишних уникальных файлов
void DuplicateSearcher::removeUniqHash(HashFiles& listCurrentHash)
{
	for (auto it = listCurrentHash.begin(); it != listCurrentHash.end();)
	{
		if (it->second.size() == 1)
			it = listCurrentHash.erase(it);
		else
			++it;
	}
}

bool DuplicateSearcher::isEnd(HashFiles& listCurrentHashl)
{
	for (auto const& [hash, files] : listCurrentHashl)
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

void DuplicateSearcher::readFileInfo(std::vector <bf::path>& filelist)
{
	for (const auto& file : filelist)
	{
		FileInfo info(file);
		info.fileData = _reader->readFile(file);
		_filesData.push_back(std::move(info));
	}
}
//void DuplicateSearcher::searchDuplicate(std::vector < bf::path>& conteiner) 
//{
//	std::unordered_map<std::string, std::vector<bf::path>> temp;
//	findConcurrence(conteiner, temp);
//	removeUniqHash(temp);
//	
//	if (isEnd(temp)) 
//	{
//		for (auto h : temp) 
//		{
//			_listDuplicate.push_back( h.second );
//		}
//	}
//	else  
//	{	
//		_currentPos += _reader->getBlockSize();
//		for (auto it : temp) 
//		{
//			searchDuplicate(it.second);
//		}
//		_currentPos -= _reader->getBlockSize();
//	}
//}


void DuplicateSearcher::searchDuplicate(std::vector <bf::path>& fileList)
{
	readFileInfo(fileList);
}




void DuplicateSearcher::print(HashFiles& listCurrentHash)
{
	for (auto const& [hash, filelist] : listCurrentHash)
	{
		std::cout << "Matches the hash: " << hash << std::endl;
		std::for_each(filelist.rbegin(), filelist.rend(), [](const bf::path& file) { std::cout << file << std::endl; });
		std::cout <<  std::endl;
	}
}
void DuplicateSearcher::print()
{
	for (auto const& filelist : _listDuplicate)
	{
		std::for_each(filelist.rbegin(), filelist.rend(), [](const bf::path& file) { std::cout << file << std::endl; });
		std::cout << std::endl;
	}
}

