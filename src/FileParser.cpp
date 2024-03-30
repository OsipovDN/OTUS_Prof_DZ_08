/*! \FileParser
	\brief source file

	Implementation FileParser.
*/

///@{
#include "FileParser.h"
namespace bf = boost::filesystem;
namespace ba = boost::adaptors;


FileParser::FileParser(int level, unsigned long long minFileSize, std::string mask):
	_level(level),
	_minFileSize(minFileSize),
	_parseLevelDir(0)
{
	_mask = std::make_unique<boost::regex>(mask);
}

void FileParser::ScanDir(const bf::path& incDir, const std::vector < bf::path>& excListDir, std::vector < bf::path>& fileList)
{
	auto filterPath = [&](const bf::path& p)->bool {return std::find(excListDir.begin(), excListDir.end(), p) == std::end(excListDir) ? true : false; };

	if (_parseLevelDir <= _level)
	{
		for (const auto& it : boost::make_iterator_range(bf::directory_iterator{ incDir }, bf::directory_iterator{})
			| ba::filtered(filterPath))
		{
			if (bf::is_directory(it.path())) 
			{
				_parseLevelDir++;
				ScanDir(it.path(), excListDir, fileList);
				_parseLevelDir--;
			}
			else
				fileList.push_back(it.path());
		}
	}
}

std::vector <bf::path> FileParser::ScanDirectories(std::vector<bf::path>& incListDir, const std::vector<bf::path>& excListDir)
{
	std::vector < bf::path> listFile;
	for (const auto& dir : incListDir) 
	{
		_parseLevelDir = 0;
		ScanDir(dir, excListDir, listFile);
	}
	return listFile;
}

std::vector < bf::path> FileParser::FilterListFile(std::vector <bf::path>& fileList) 
{
	std::vector <bf::path> listFile;
	auto FilterSize = [&](const bf::path& p)->bool {return file_size(p) > _minFileSize; };
	auto FilterMask = [&](const bf::path& p)->bool {return !boost::regex_match(p.filename().string(), *_mask); };

	for (const auto& it : fileList
		| ba::filtered(FilterSize)
		| ba::filtered(FilterMask))
	{
		listFile.push_back(it);
	}
	return listFile;

}

///@}



