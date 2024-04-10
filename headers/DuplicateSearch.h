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
#include <iostream>
//Boost
#include <boost/filesystem.hpp>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

/*! \brief Class "DuplicateSearcher".

	The class implements methods for searching for duplicate files in directories
	with specified user-defined comparison parameters
*/
namespace bf = boost::filesystem;
///<Node -пара позиции блока и соответствующих данной позиции хэшей и списков файлов соответствующих этим хешам 
using Node = std::pair<unsigned long long, std::pair<std::string, std::vector<bf::path>>>;
///<HashMap- контейнер содержащий значения хэшей и списки файлов им соответствующие
using HashMap=std::unordered_map<std::string, std::vector<bf::path>>;

/*! \brief EnumClass "Hash".

	The EnumClass types of hash functions.
	*/
enum class Hash {
	CRC32,
	MD5,
	NONE
};

class DuplicateSearcher {
private:
	
	unsigned long long _blockSize;			///<block size for comparison
	unsigned long long _currentPos = 0;		///<the current position of the block
	char* _buf;								///<block
	Hash _hash;								///<type of hash function
	std::vector <std::vector <bf::path>> _listDuplicate;		///<list of duplicate files;
	std::stack <Node> _stack;				///<Stack for traversing the block tree
	HashMap _currentBlock;					///<A list of files corresponding to the current block

	std::string getCrc32(const char* buf, size_t buf_size)
	{
		boost::crc_32_type crc_hash;
		crc_hash.process_bytes(buf, buf_size);
		return std::to_string(crc_hash.checksum());
	};

	std::string getMD5(const char* buf, size_t buf_size)
	{
		boost::uuids::detail::md5 md5_hash;
		md5_hash.process_bytes(buf, buf_size);
		boost::uuids::detail::md5::digest_type digest;
		md5_hash.get_digest(digest);
		const auto intDigest = reinterpret_cast<const int*>(&digest);
		std::string result;
		boost::algorithm::hex(intDigest, intDigest + (sizeof(boost::uuids::detail::md5::digest_type) / sizeof(int)), std::back_inserter(result));
		return result;
	};

	/*! The method calculates the hash according to the hash function selected by the user.
		\param block - the block where the hash is calculated.
		\return std::string - hash.
	*/
	std::string getHash(std::string& block);
	/*! The method Determines the type of hash function selected by the user.
		\param type - user-defined type.
		\return Hash - hash type.
	*/
	Hash hashType(std::string& type) noexcept;
	/*! The method reads a block from the specified file.
		\param path - the file.
		\return std::string - the read block.
	*/
	std::string readBlockInFile(bf::path& path);

	/*! The method checks for the presence of this hash in the general list. If such a hash occurs, the file is added to the general list.
		\param hash - the hash you are looking for.
		\param file - tthe file that this hash belongs to.
	*/
	void checkHashInList(std::string& hash, bf::path& file);
	/*! This method reads a block of the current file from
		a given position and compares it with the available blocks in the current hash list.
		\param listFile -The file list being checked.
	*/
	void findConcurrence(std::vector<bf::path>& listFile);
	/*! 
		This method removes unique hashes from the list of current ones
	*/
	void removeUniqHash();
	/*! The method checks for the end of the file. If the current position of the block being
	checked exceeds the file size, the file is excluded from further comparison.
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
	/*! 
	The method writes nodes of the current processed position of the read block to the stack.
	*/
	void pushInStack();
	/*!the method searches for duplicate files in the specified list of files.
		\param listPath - list of files to search for duplicates.
	*/
	void searchDuplicate(std::vector<bf::path> &listPath);
	/*!The method outputs all files according to the current hash
		\param listCurrentHash - List of all hashes of the current block position
	*/
	void print(HashMap& listCurrentHashl);
	/*
		!The method outputs grouped lists of duplicate files
	*/
	void print();
	std::vector <std::vector <bf::path>> getList() { return _listDuplicate; }
	
};
///@}