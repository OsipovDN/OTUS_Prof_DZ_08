#pragma once
/*! \file
	\brief header file FileScaner.h
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

/*! \brief Class FileScaner.

	Extracts files from specified directories, taking into account user-defined external features
*/

namespace bf = boost::filesystem;
namespace ba = boost::adaptors;

class FileScaner {
private:
	int _depth;								///<The depth of the passage of the dirrectory
	unsigned long long _minFileSize;		///<Minimum size of scanned files
	int _parseLevelDir;						///<Current directory level
	std::unique_ptr<boost::regex> _mask;	///<File mask for scanning
	std::vector <bf::path> _includeDirs;	///<Directories for scanning
	std::vector <bf::path> _excludedDirs;	///<Execluded directories from scanning
	std::vector <bf::path> _files;			///<result scanning

	/*! Extracts files from a directory.
		\param includeDirs - the directory to scan.
	*/
	void getFileFromDir(const bf::path& includeDirs);
	/*! The method filters files based on user parameters*/
	void startScanning();

public:
	/*!Constructor of a class for forming an object.
		\param level - directory scanning depth.
		\param minFileSize - maximum file size for scanning.
		\param mask - file name mask.
	*/
	FileScaner(int level, unsigned long long minFileSize, std::string mask);
	/*! The method retrieves files from the specified directory.
		\param incListDir - the list of directory to scan.
		\param excListDir - the list of directories excluded for scanning.
	*/
	void scanDirectories(std::vector<bf::path>& incListDir, const std::vector<bf::path>& excListDir);
	/*! The method returns a list of files that meets the user's parameters
		\return std::string - list of files.
	*/
	std::vector < bf::path> getFiles() { return _files; };
};
///@}
