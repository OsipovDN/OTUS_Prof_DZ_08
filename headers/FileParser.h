#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/regex.hpp>

namespace bf = boost::filesystem;
namespace ba = boost::adaptors;

class FileParser {
private:
	int level;
	unsigned long long min_file_size;
	int fact_lvl_dir;
	std::unique_ptr<boost::regex> mask;
	void ScanDir(const bf::path& in_dir, const std::vector < bf::path>& ex_dir, std::vector < bf::path>& list_file);

public:
	FileParser(int l, unsigned long long mfs, std::string m);
	std::vector <bf::path> ScanListDir(std::vector<bf::path>& list_dir, const std::vector<bf::path>& ex_dir);
	std::vector <bf::path> ScanListFile(std::vector <bf::path>& file_list);
	


};
