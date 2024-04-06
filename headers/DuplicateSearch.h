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
#include "FileReader.h"
#include "md5.h"
#include "crc32.h"

/*! \brief Class "DuplicateSearcher".
	The class implements methods for searching for duplicate files in directories
	with specified user-defined comparison parameters
*/
namespace bf = boost::filesystem;
//using HashFiles = std::unordered_map<std::string, std::vector<bf::path>>;


struct FileInfo
{
	bf::path filePath;
	size_t fileSize;
	std::string fileName;
	std::vector<std::string> fileData;

	FileInfo(const bf::path& path) :
		filePath(path), 
		fileSize(bf::file_size(path)),
		fileName(path.filename().string()) 
	{
		fileData.reserve(fileSize);
	}
	std::string getBlock(std::streampos pos) { return fileData[pos]; }
};

/*! \brief EnumClass "Hash".
	The EnumClass types of hash functions.
*/
enum class Hash {
	CRC32,
	MD5,
	NONE
};

using HashNode= std::unordered_map<std::string, std::vector<FileInfo>>;

class DuplicateSearcher {
private:
	std::unique_ptr <FileReader> _reader;					///<Reader of file
	std::vector <FileInfo> _filesData;
	std::vector <std::vector <FileInfo>> _listDuplicate;	///<list of duplicate files
	Hash _hash;												///<type of hash function
	//std::streampos _currentPos;								///<the current position of the block
	std::size_t _currentPos;								///<the current position of the block
	unsigned long long _blockSize;							///<block size for comparison

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

	/*! The method checks for the presence of this hash in the general list. If such a hash occurs, the file is added to the general list.
		\param hash - the hash you are looking for.
		\param file - tthe file that this hash belongs to.
		\param listCurrentHash - List of all hashes of the current block position.
	*/
	void checkHashInList(std::string& hash, FileInfo& file, HashNode& listCurrentHash);

	/*! This method removes unique hashes from the list of current ones
		\param listCurrentHash - List of all hashes of the current block position.
	*/
	void removeUniqHash(HashNode& listCurrentHash);
	/*! The method checks for the end of the file. If the current position of the block being
	checked exceeds the file size, the file is excluded from further comparison.
		\param listCurrentHash - List of all hashes of the current block position.
		\return bool - Indicates the end of the file.
	*/
	bool isEnd(HashNode& listCurrentHashl);

	void readFileInfo(std::vector <bf::path>& fileList);

public:
	/*!Constructor of a class for forming an object.
		\param blockSize - block size for comparison.
		\param hash - type of hash function.
	*/
	DuplicateSearcher(std::unique_ptr<FileReader> reader, std::string& hash);
	/*!Destructor.
		frees up the memory allocated for the block being compared
	*/
	~DuplicateSearcher()=default;

	/*!the method searches for duplicate files in the specified list of files.
		\param listPath - list of files to search for duplicates.
	*/
	void searchDuplicate(std::vector<bf::path>& listPath);
	/*!the method searches for duplicate files in the specified list of files.
		\return std::vector <std::vector <bf::path>> - list of duplicate files.
	*/
	std::vector <std::vector <FileInfo>> getList() { return _listDuplicate; }
	/*!The method outputs all files according to the current hash
		\param listCurrentHash - List of all hashes of the current block position
	*/
	void print(HashNode& listCurrentHashl);
	/*!The method outputs grouped lists of duplicate files
	*/
	void print();
	

};
///@}