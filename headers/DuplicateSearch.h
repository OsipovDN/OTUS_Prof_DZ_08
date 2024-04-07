#pragma once
/*! \file
	\brief header file DuplicateSearch.h
*/

///@{

//STL
#include <fstream>
#include <memory>
#include <unordered_map>
#include <stack>
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
using HashNode=std::unordered_map<std::string, std::vector<bf::path>>;

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
	std::stack <HashNode> _stack;
	HashNode _currentBlock;

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

	/*! The method checks for the presence of this hash in the general list. If such a hash occurs, the file is added to the general list.
		\param hash - the hash you are looking for.
		\param file - tthe file that this hash belongs to.
		\param listCurrentHash - List of all hashes of the current block position.
	*/
	void checkHashInList(std::string& hash, bf::path& file);
	/*! This method reads a block of the current file from
		a given position and compares it with the available blocks in the current hash list.
		\param listFile -The file list being checked.
		\param listCurrentHash - List of all hashes of the current block position.
	*/
	void findConcurrence(std::vector<bf::path>& listFile);
	/*! This method removes unique hashes from the list of current ones
		\param listCurrentHash - List of all hashes of the current block position.
	*/
	void removeUniqHash();
	/*! The method checks for the end of the file. If the current position of the block being
	checked exceeds the file size, the file is excluded from further comparison.
		\param listCurrentHash - List of all hashes of the current block position.
		\return bool - Indicates the end of the file.
	*/
	bool isEnd();

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
	void searchDuplicate(std::vector<bf::path> &listPath);
	/*!The method outputs all files according to the current hash
		\param listCurrentHash - List of all hashes of the current block position
	*/
	void print(HashNode& listCurrentHashl);
	/*!The method outputs grouped lists of duplicate files
	*/
	void print();
	std::vector <std::vector <bf::path>> getList() { return _listDuplicate; }
	
};
///@}