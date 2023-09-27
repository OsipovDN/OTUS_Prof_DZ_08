#pragma once
#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

namespace bf = boost::filesystem;

class FileParser {
private:
	std::vector < bf::path> include_dir;
	std::vector < bf::path> excluded_dir;
	int level;
	long long min_file_size;
	std::unique_ptr<boost::regex >mask;

public:
	FileParser(std::vector < bf::path> include, std::vector < bf::path> exclude, int l, long long mfs, std::string m) :
		include_dir(include), excluded_dir(exclude), level(l), min_file_size(mfs)
	{
		mask=std::make_unique<boost::regex>(m);
	}
};
