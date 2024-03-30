#pragma once
/*! \file
	\brief header file DuplicateSearch.h
*/

///@{

//STL
#include <fstream>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <string>
#include <algorithm>
//Boost
#include <boost/filesystem.hpp>
//Hash
#include "md5.h"
#include "crc32.h"

/*! \brief Class "DuplicateSearcher".

	The class implements methods for searching for duplicate files in directories
	with specified user-defined comparison parameters
*/
namespace bf = boost::filesystem;
using HashFiles=std::unordered_map<std::string, std::vector<bf::path>>;

class DuplicateSearcher {
private:
	/*! \brief EnumClass "Hash".

	The EnumClass types of hash functions.
	*/
	enum class Hash {
		CRC32,
		MD5,
		NONE
	};
	unsigned long long _blockSize;		//block size for comparison
	std::streampos _currentPos = 0;		//the current position of the block
	char* _buf;		//block
	Hash _hash;		//type of hash function
	std::vector <std::vector <bf::path>> _listDuplicate;		//list of duplicate files;

	/*! The method calculates the hash according to the hash function selected by the user.
		\param block - the block where the hash is calculated.
		\return std::string - hash.
	*/
	std::string getHash(std::string& block);
	/*! The method Determines the type of hash function selected by the user.
		\param type - user-defined type.
		\return Hash - hash.
	*/
	Hash hashType(std::string& type) noexcept;
	/*! The method checks the block size to match the one specified by the user.
		\param file - the file in which the check is performed.
		\return unsigned long long - block size.
	*/
	unsigned long long checkSizeBlock(std::ifstream& file);
	/*! The method reads a block from the specified file.
		\param path - the file.
		\return std::string - the read block.
	*/
	std::string readBlockInFile(bf::path& path);

	//void scanBlock(std::vector<bf::path>& list_path);
	void checkHashInList(std::string& hash, bf::path& file, HashFiles& listCurrentHash);

	void findConcurrence(std::vector<bf::path>& listFile, HashFiles& listCurrentHashl);
	void cleanList(HashFiles& listCurrentHash);
	bool isEnd(HashFiles& listCurrentHashl);

public:
	/*!Constructor of a class for forming an object.
		\param blockSize - block size for comparison.
		\param hash - type of hash function.
	*/
	DuplicateSearcher(unsigned long long blockSize, std::string& hash);
	/*!Destructor.
		frees up the memory allocated for the block being compared
	*/
	~DuplicateSearcher();
	
	/*!the method searches for duplicate files in the specified list of files.
		\param listPath - list of files to search for duplicates.
	*/
	void searchDuplicate(std::vector<bf::path> listPath);


	//Вывод на печать списка дубликатов (вспомогательные функции)
	void print(HashFiles& l);
	void print(std::vector<std::vector<bf::path>>& l);
	std::vector <std::vector <bf::path>> getList() { return _listDuplicate; }
	
};
///@}