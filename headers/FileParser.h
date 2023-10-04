﻿#pragma once
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
	unsigned long long min_file_size;
	std::unique_ptr<boost::regex> mask;

public:
	FileParser(int l, long long mfs, std::string m) :
		level(l), min_file_size(mfs)
	{
		fact_lvl_dir = 0;
		mask = std::make_unique<boost::regex>(m);
	}

	void ScanDir(const bf::path& in_dir, const std::vector < bf::path>& ex_dir, std::vector < bf::path>& list_file) {
		auto filterPath = [&](const bf::path& p)->bool {return  (std::find(ex_dir.begin(), ex_dir.end(), p) == std::end(ex_dir)) ? true : false; };
		auto filterMask = [&](const bf::path& p)->bool {return boost::regex_match(p.filename().string(), *mask); };

		if (level > fact_lvl_dir) {
			for (bf::directory_iterator it{ in_dir }; it != bf::directory_iterator(); ++it
				| ba::filtered(filterPath)
				| ba::filtered(filterMask)) {
				if (bf::is_directory(it->path())) {
					fact_lvl_dir++;
					ScanDir(it->path(), ex_dir, list_file);
					fact_lvl_dir--;
				}
				else
					list_file.push_back(it->path());
			}
		}
	}

	std::vector < bf::path> ScanListDir(std::vector<bf::path>& list_dir, const std::vector<bf::path>& ex_dir) {
		std::vector < bf::path> file_list;
		for (const auto& dir : list_dir) {
			fact_lvl_dir = 0;
			ScanDir(dir, ex_dir, file_list);
		}
		return std::move(file_list);
	}
};
