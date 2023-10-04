#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/regex.hpp>

namespace bf = boost::filesystem;
namespace ba = boost::adaptors;

class FileParser {
private:
	int level;
	int fact_lvl_dir;
	unsigned long long min_file_size;
	std::unique_ptr<boost::regex> mask;

public:
	FileParser(int l, long long mfs, std::string m) :
		level(l), fact_lvl_dir(0), min_file_size(mfs) {

		mask = std::make_unique<boost::regex>(m);
	}

	std::vector <bf::path> ScanListDir(std::vector<bf::path>& list_dir, const std::vector<bf::path>& ex_dir) {
		std::vector < bf::path> file_list;
		for (const auto& dir : list_dir) {
			fact_lvl_dir = 0;
			ScanDir(dir, ex_dir, file_list);
		}
		return std::move(file_list);
	}

	bool ScanFile(const bf::path& path) {
		return boost::regex_match(path.filename().string(), *mask);
		
	}


	void ScanDir(const bf::path& in_dir, const std::vector < bf::path>& ex_dir, std::vector < bf::path>& list_file) {

		auto filterPath = [&](const bf::path& p)->bool {return std::find(ex_dir.begin(), ex_dir.end(), p) == std::end(ex_dir) ? true : false; };

		if (level > fact_lvl_dir) {
			for (const auto& it: boost::make_iterator_range(bf::directory_iterator{ in_dir }, bf::directory_iterator{})
				| ba::filtered(filterPath)) {
				if (bf::is_directory(it.path())) {
					fact_lvl_dir++;
					ScanDir(it.path(), ex_dir, list_file);
					fact_lvl_dir--;
				}
				else
					if (ScanFile(it.path())) {
						list_file.push_back(it.path());
					}
				
			}
		}
	}


};
