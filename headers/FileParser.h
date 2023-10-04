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

	void ScanDir(const bf::path& in_dir,const std::vector < bf::path>& ex_dir, std::vector < bf::path>& list_file) {

		auto filterSize = [&](bf::directory_iterator& it) {return it->path().size() < min_file_size; };
		
		if (level > fact_lvl_dir) {
			bf::directory_iterator it{ in_dir };
			while (it != bf::directory_iterator()
				| ba::filtered(filterSize)
				)
			{
				if (bf::is_directory(it->path().string())) {
					fact_lvl_dir++;
					ScanDir(it->path(), ex_dir, list_file);
					fact_lvl_dir--;
				}
				else
					list_file.push_back(it->path());
				it++;
			}
		}
	}
	//boost::smatch same_mask;
	//	for (const auto& dir : in_dir)
	//		/*
	//		| ba::filtered([&](const bf::path& path) { return boost::regex_match(path.filename().string(), same_mask, *mask); })*/ {
	//	}
	//}

	std::vector < bf::path> ScanListDir(std::vector<bf::path>& list_dir, const std::vector<bf::path>& ex_dir ) {
		std::vector < bf::path> file_list;
		for (const auto& dir : list_dir) {
			fact_lvl_dir = 0;
			ScanDir(dir, ex_dir, file_list);
		}
		return std::move(file_list);
	}
};
