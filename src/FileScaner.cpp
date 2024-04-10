/*! \FileScaner
	\brief source file

	Implementation FileScaner.
*/

///@{
#include "FileScaner.h"

FileScaner::FileScaner(int depth, unsigned long long minFileSize, std::string mask) :
	_depth(depth),
	_minFileSize(minFileSize),
	_mask(mask),
	_parseLevelDir(0)
{
	
}

void FileScaner::getFileFromDir(const bf::path& includeDirs)
{
	auto isAnExcluded = [&](const bf::path& path)->bool {
		return std::find(_excludedDirs.begin(), _excludedDirs.end(), path) == std::end(_excludedDirs) ? true : false; };

	if (_parseLevelDir <= _depth)
	{
		for (const auto& it : boost::make_iterator_range(bf::directory_iterator{ includeDirs }, bf::directory_iterator{})
			| ba::filtered(isAnExcluded))
		{
			if (bf::is_directory(it.path()))
			{
				_parseLevelDir++;
				getFileFromDir(it.path());
				_parseLevelDir--;
			}
			else
				_files.emplace_back(it.path());
		}
	}
}

void FileScaner::startScanning()
{
	std::vector <bf::path> tempfileList;
	auto FilterSize = [&](const bf::path& p)->bool {return file_size(p) > _minFileSize; };
	auto FilterMask = [&](const bf::path& p)->bool {return !boost::regex_match(p.filename().string(), _mask); };

	for (const auto& it : _files
		| ba::filtered(FilterSize)
		| ba::filtered(FilterMask))
	{
		tempfileList.push_back(it);
	}
	_files = tempfileList;
}

void FileScaner::scanDirectories(std::vector<bf::path>& inclDirs, const std::vector<bf::path>& exclDirs)
{
	_includeDirs = inclDirs;
	_excludedDirs = exclDirs;
	for (const auto& dir : _includeDirs)
	{
		_parseLevelDir = 0;
		getFileFromDir(dir);
		startScanning();
	}
}





