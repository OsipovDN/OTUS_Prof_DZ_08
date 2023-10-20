#include "FileParser.h"
#include "DuplicateSearch.h"
#include "md5.h"


//STL
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
//BOOST
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp> 
#include <boost/regex.hpp>

namespace bf = boost::filesystem;
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	std::vector<bf::path> incl;
	std::vector<bf::path> excl;
	int lvl;
	unsigned long long size;
	std::string mask;

	long long block_size;
	std::string hash;


	po::options_description desc("Options");
	desc.add_options()
		("help,h", "produce help")
		("include,i", po::value<std::vector<bf::path>>()->multitoken(), "Include directories for scanning")
		("exclude,e", po::value<std::vector<bf::path>>()->multitoken(), "Exclude directories from scanning")
		("level,l", po::value<int>()->default_value(0), "One for all directories, 0 - only the specified directory without nested")
		("Min_file_size,s", po::value<unsigned long long>()->default_value(1), "By default, all files larger than 1 byte are checked")
		("file_mask,m", po::value<std::string>()->default_value("(\\w+)(\\d+).(\\w+)(\\d+)"), "File mask") //(\\w+)(\\d+).(\\w+)(\\d+)
		("Block_size,S", po::value<unsigned long long>()->default_value(5), "The size of the block used to read files")
		("hash_type,t", po::value<std::string>()->default_value("crc32"), "Hashing algorithm type: md5, crc32");

	po::variables_map vm;
	po::store(parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
		std::cout << desc << '\n';

	if (vm.count("include")) {
		std::cout << "include dir: " << std::endl;
		for (auto it : vm["include"].as<std::vector<bf::path>>()) {
			std::cout << it << std::endl;
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
	std::cout << "Min dir level to search: " << lvl << std::endl;

	size = vm["Min_file_size"].as<unsigned long long>();
	std::cout << "Min file size to search: " << size << std::endl;

	mask = vm["file_mask"].as<std::string>();
	std::cout << "Mask for path: " << mask << std::endl;

	block_size = vm["Block_size"].as<unsigned long long>();
	std::cout << "Block_size is: " << block_size << std::endl;

	hash = vm["hash_type"].as<std::string>();
	std::cout << "Hash func is: " << hash << std::endl;


	//Контейнер отфильтрованных файлов
	std::vector < bf::path> file_list;
	//Контейнер для дубликатов
	std::map <std::string, bf::path> duplicate;

	//Парсер дирректорий
	FileParser parser(lvl, size, mask);
	//Поисковик  дупликатов
	DuplicateSearch searcher(block_size, hash);

	//Фильтруем директории по глубине сканирования и исключая ненужные
	file_list = parser.ScanListDir(incl, excl);
	//Фильтруем по маске названия файла и по размеру файла 
	parser.FilterListFile(file_list);

	searcher.search(file_list);




	
	/*std::cout << "--------------" << std::endl;
	const std::string str{ "word" };
	std::string fact_hash = md5(str);
	std::cout << fact_hash << std::endl;
	std::cout <<"------" << std::endl;
	fact_hash = md5(str);
	std::cout << fact_hash << std::endl;
	std::cout << "------" << std::endl;*/
	


	std::cout << "--------------" << std::endl;
	std::for_each(file_list.rbegin(), file_list.rend(),[](const bf::path & file){ std::cout << file << std::endl; });

	std::cout << "--------------" << std::endl;







	return 0;
}
