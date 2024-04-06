
//STL
#include <iostream>
#include <memory>
#include <vector>
//BOOST
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp> 
#include <boost/regex.hpp>

#include "FileScaner.h"
#include "DuplicateSearch.h"

namespace bf = boost::filesystem;
namespace po = boost::program_options;



int main(int argc, char* argv[])
{

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
	else {

		std::vector<bf::path> incl;
		std::vector<bf::path> excl;
		int lvl;
		unsigned long long fileSize;
		std::string mask;

		long long blockSize;
		std::string hash;
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

		fileSize = vm["Min_file_size"].as<unsigned long long>();
		std::cout << "Min file size to search: " << fileSize << std::endl;

		mask = vm["file_mask"].as<std::string>();
		std::cout << "Mask for path: " << mask << std::endl;

		blockSize = vm["Block_size"].as<unsigned long long>();
		std::cout << "Block_size is: " << blockSize << std::endl;

		hash = vm["hash_type"].as<std::string>();
		std::cout << "Hash func is: " << hash << std::endl;


		//Контейнер отфильтрованных файлов
		std::vector < bf::path> fileList;
		//Контейнер для дубликатов
		std::vector <std::vector <FileInfo>> duplicateFile;

		/*
		Парсер проводит фильтрацию файлов по указанным пользователем параметрам
		- по глубине прохода дирикторий
		- по размеру файлов
		- по маске файлов
		*/
		FileScaner parser(lvl, fileSize, mask);
		//Фильтруем директории по глубине сканирования и исключая ненужные
		parser.scanDirectories(incl, excl);
		/*
		Поисковик ищет дубликаты файлов с использованием пользовательских параметров
		- размер сканиремых блоков в файле
		- hash функция для алгоритма сравнения
		*/
		fileList = parser.getFiles();
		auto reader = std::make_unique<FileReader>(blockSize);
		DuplicateSearcher searcher(std::move(reader), hash);
		/*
		На данной итерации мы получаем полный список всех фалов в просканированных дирректориях отвечающих
		параметрам заданным пользователем
		*/
		//printFileList(filterfileList);
		
		//Ищем дубликаты
		searcher.searchDuplicate(fileList);
		duplicateFile = searcher.getList();
		//Вывод в консоль
		searcher.print();

	}

	return 0;
}
