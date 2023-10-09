#pragma once
#include <string>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>


namespace bf = boost::filesystem;



class DuplicateSearch {
private:
	enum class Hash {
		CRC32,
		MD5,
		NONE
	};
	unsigned long long size_block;
	Hash h;
	std::map<std::string, std::vector<bf::path>> list;

	Hash HashType(std::string& type) noexcept {
		if (type == "MD5")
			return Hash::MD5;
		else if (type == "CRC32")
			return Hash::CRC32;
		else
			return Hash::NONE;
	}
public:
	DuplicateSearch(unsigned long long size, std::string& hash) :size_block(size) {
		boost::algorithm::to_upper(hash);
		h = HashType(hash);
	}



	//TODO
	//Доработать код и внедрить его

	//void searh() {
	//	setlocale(LC_ALL, "rus");

	//	const int buf = 2;
	//	char b[buf];
	//	std::ifstream in("C:\\Users\\Дмитрий\\source\\repos\\ConsoleApplication3\\Debug\\8.txt",
	//		std::ios_base::in | std::ios_base::binary);

	//	if (!in.is_open()) { // если файл не открыт
	//		std::cout << "Файл не может быть открыт!\n"; // сообщить об этом
	//		return 1;
	//	}

	//	while (!in.eof()) {
	//		in.read(b, buf); // считали строку из файла
	//		std::cout << b << std::endl; // напечатали эту строку
	//	}

	//	in.close();
	//}
};