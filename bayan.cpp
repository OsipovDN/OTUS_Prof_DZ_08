//TODO
/*
При установке размера блоков для чтения файлов больше 1 байта, вывод списка дубликатов будет пустой.
Непонятно почему. В частности в списке отсутствуют дубликаты пдф файлов. Допускаю , что причина при считывании последних
блоков файлов при дозаписывании не кратного блока нулями происходит какая-то ошибка и файлы не считываются корректно из-за чего
они не попадают в общий список дубликатов. Речь идет  о работе методы void DuplicateSearcher::searchDuplicate(std::vector < bf::path>& conteiner),
который в процессе вызывает метод std::string DuplicateSearcher::readBlockInFile(bf::path& path)
*/


//STL
#include <iostream>
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

void printListDuplicate(std::vector <std::vector <bf::path>>& listDuplicate) {
	for (const auto& list : listDuplicate) {
		for (const auto& file: list) {
			std::cout << file << std::endl;
		}
		std::cout<<std::endl;
	}
}

void printFileList(std::vector < bf::path> list)
{
	std::for_each(list.rbegin(), list.rend(), [](const bf::path& file) { std::cout << file << std::endl; });
}

int main(int argc, char* argv[])
{
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "produce help")
		("include,i", po::value<std::vector<bf::path>>()->multitoken(), "Include directories for scanning")
		("exclude,e", po::value<std::vector<bf::path>>()->multitoken(), "Exclude directories from scanning")
		("level,l", po::value<int>()->default_value(0), "One for all directories, 0 - only the specified directory without nested")
		("Min_file_size,s", po::value<unsigned long long>()->default_value(1), "By default, all files larger than 1 byte are checked")
		("file_mask,m", po::value<std::string>()->default_value("([a-zA-Z0-9]+).([a-zA-Z0-9]+)"), "File mask") //(\\w+)(\\d+).(\\w+)(\\d+)
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
		std::vector <std::vector <bf::path>> duplicateFile;

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
		/*
		На данной итерации мы получаем полный список всех фалов в просканированных дирректориях отвечающих
		параметрам заданным пользователем
		*/
		//printFileList(filterfileList);
		
		//Ищем дубликаты
		DuplicateSearcher searcher(blockSize, hash);
		if (blockSize <= 10)
		{
			std::cout << "The scan is running. Please wait ..." << std::endl;
		}
		searcher.searchDuplicate(fileList);
		duplicateFile = searcher.getList();
		//Вывод в консоль
		printListDuplicate(duplicateFile);

	}

	return 0;
}
