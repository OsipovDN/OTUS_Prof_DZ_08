#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/regex.hpp>

namespace bf = boost::filesystem;
namespace ba = boost::adaptors;

class FileParser {
private:
	int level;
	int fact_lvl_dir;
	long long min_file_size;
	std::unique_ptr<boost::regex> mask;

public:
	FileParser(int l, long long mfs, std::string m) :
		level(l), min_file_size(mfs)
	{
		fact_lvl_dir = 0;
		mask = std::make_unique<boost::regex>(m);
	}

	void ScanDir(const bf::path& in_dir, const std::vector < bf::path>& ex_dir, std::vector < std::string>& list_file) {
		
		if (level > fact_lvl_dir) {
			bf::directory_iterator it{ in_dir };
			while (it != bf::directory_iterator()) {
				if (bf::is_directory(it->path().string())) {
					fact_lvl_dir++;
					std::cout << it->path().string() << "-----is_dir_lvl " << fact_lvl_dir << std::endl;
					ScanDir(it->path().string(), ex_dir, list_file);	
					fact_lvl_dir--;
				}
				else 
					list_file.push_back(it->path().string());
				it++;
			}
		}
		else
			return;
	}
	//boost::smatch same_mask;
	//	for (const auto& dir : in_dir)
	//		/*| ba::filtered([&](const bf::path& path) { return bf::file_size(path) > min_file_size; })
	//		| ba::filtered([&](const bf::path& path) { return boost::regex_match(path.filename().string(), same_mask, *mask); })*/ {
	//	}
	//}

	std::vector < std::string>&& ScanListDir(std::vector<bf::path>& list_dir, const std::vector<bf::path>& ex_dir) {
		std::vector < std::string> temp;
		for (const auto& dir : list_dir) {
			fact_lvl_dir = 0;
			ScanDir(dir, ex_dir, temp);
		}
		return std::move(temp);
	}
};
