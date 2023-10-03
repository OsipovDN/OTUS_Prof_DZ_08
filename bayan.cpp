#include "FileParser.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp> 
#include <boost/regex.hpp>
#include <iostream>
#include <vector>
#include <string>

namespace bf = boost::filesystem;
namespace po = boost::program_options;


int main(int argc, char* argv[])
{
	std::vector<bf::path> incl;
	std::vector<bf::path> excl;
	int lvl;
	long long size;
	std::string mask;


	po::options_description desc("Options");
	desc.add_options()
		("help,h", "produce help")
		("include,i", po::value<std::vector<bf::path>>()->multitoken(), "Include directories for scanning")
		("exclude,e", po::value<std::vector<bf::path>>()->multitoken(), "Exclude directories from scanning")
		("level,l", po::value<int>()->default_value(5), "One for all directories, 0 - only the specified directory without nested")
		("Min_file_size,s", po::value<long long>()->default_value(1), "By default, all files larger than 1 byte are checked")
		("file_mask,m", po::value<std::string>()->default_value(".*."), "File mask")
		("Block_size,S", po::value<int>()->default_value(5), "The size of the block used to read files")
		("hash_type,t", po::value<std::string>()->default_value("crc32"), "Hashing algorithm type: md5, crc32");

	po::variables_map vm;
	po::store(parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
		std::cout << desc << '\n';

	if (vm.count("include")) {
		std::cout << "include dir: " << std::endl;
		for (auto it : vm["include"].as<std::vector<bf::path>>()) {
				std::cout << it<< std::endl;
			}
		incl = vm["include"].as<std::vector<bf::path>>();
	}

	if (vm.count("exclude")) {
		std::cout << "exclude dir: " << std::endl;
		for (auto it : vm["exclude"].as<std::vector<bf::path>>()) {
			std::cout << it << std::endl;
		}
		excl = vm["exclude"].as<std::vector<bf::path>>();
	}
	
	lvl = vm["level"].as<int>();
	std::cout << "Min file size to search: " << std::endl;
	std::cout << lvl << std::endl;
	
	size = vm["Min_file_size"].as<long long>();
	std::cout << "Min file size to search: " << std::endl;
	std::cout << size << std::endl;
	
	mask=vm["file_mask"].as<std::string>();
	std::cout << "Mask for path: " << std::endl;
	std::cout << mask << std::endl;
	
	FileParser parser(lvl, size, mask);

	std::vector < std::string> file_list;
	file_list = parser.ScanListDir(incl, excl);

	std::cout << "--------------" << std:: endl;
	for (const auto& it : file_list) {
		std::cout << it << std:: endl;
	}
	std::cout << "--------------" << std::endl;


}
