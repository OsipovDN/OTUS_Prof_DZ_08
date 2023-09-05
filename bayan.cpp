﻿#include "bayan.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp> 
#include <boost/regex.hpp>
#include <iostream>
#include <vector>
#include <string>

namespace bf = boost::filesystem;
namespace po = boost::program_options;

void reg(std::string){}

int main(int argc, char* argv[])
{
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "produce help")
		("include,i", po::value<std::vector<std::string>>()->multitoken(), "Include directories for scanning")
		("exclude,e", po::value<std::vector<std::string>>()->multitoken(), "Exclude directories from scanning")
		("level,l", po::value<int>()->default_value(5), "One for all directories, 0 - only the specified directory without nested")
		("Min_file_size,s", po::value<long long>()->default_value(1), "By default, all files larger than 1 byte are checked")
		("file_mask,m", po::value<std::string>()->default_value(".*.")->notifier(reg), "File mask")
		("Block_size,S", po::value<int>()->default_value(5), "The size of the block used to read files")
		("hash_type,t", po::value<std::string>()->default_value("crc32"), "Hashing algorithm type: md5, crc32")
		;


	po::variables_map vm;
	po::store(parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
		std::cout << desc << '\n';
	else if (vm.count("optimization")) {

		for (auto it : vm["optimization"].as<std::vector<bf::path>>()) {
			std::cout << "readfrom: " << it << std::endl;
		}
	}


}
