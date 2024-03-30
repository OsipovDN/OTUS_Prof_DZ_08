#pragma once
/*! \file
	\brief header file FileParser.h
*/

///@{
//STL
#include <iostream>
#include <string>
#include <memory>
//Boost
#include <boost/filesystem.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/regex.hpp>

/*! \brief Class FileParser.

	Filters files by the key criteria specified by the user
*/

namespace bf = boost::filesystem;
namespace ba = boost::adaptors;

class FileParser {
private:
	int _level;
	unsigned long long _minFileSize;
	int _parseLevelDir;
	std::unique_ptr<boost::regex> _mask;
	/*!Constructor of a class for forming an object.
		\param incDir - the directory to scan.
		\param excDir - list of directories excluded from scanning.
		\param listFile - list of files.
	*/
	void ScanDir(const bf::path& incDir, const std::vector < bf::path>& excListDir, std::vector < bf::path>& fileList);

public:
	/*!Constructor of a class for forming an object.
		\param level - directory scanning depth.
		\param maxFileSize - maximum file size for scanning.
		\param mask - file name mask.

	*/
	FileParser(int level, unsigned long long minFileSize, std::string mask);
	/*! A method The method of scanning a given directory.
		\param incListDir - the list of directory to scan.
		\param excListDir - the list of directories excluded for scanning.
		\return std::vector <bf::path> - the list of files located in the scanned directories.
	*/
	std::vector <bf::path> ScanDirectories(std::vector<bf::path>& incListDir, const std::vector<bf::path>& excListDir);
	/*! A method that provides save data in a file.
		\param fileList - the list of files that need to be filtered.
		\return std::vector <bf::path> - a list of files filtered by the initial characteristics of the user.
	*/
	std::vector < bf::path> FilterListFile(std::vector <bf::path>& fileList);
};
///@}
