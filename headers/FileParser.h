#pragma once
#include <string>
#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;

class FileParser {
private:
	std::vector < bf::path> include_dir;
	std::vector < bf::path> excluded_dir;
	int level;
	long long min_file_size;
	std::string mask;


public:
	FileParser(std::vector < bf::path> include, std::vector < bf::path> exclude, int l, long long mfs, std::string m) :
		include_dir(include), excluded_dir(exclude), level(l), min_file_size(mfs), mask(m)
	{


	}
};
