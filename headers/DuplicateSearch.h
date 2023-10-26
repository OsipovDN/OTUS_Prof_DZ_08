#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <tuple>

#include "md5.h"
#include "crc32.h"

#include <boost/filesystem.hpp>


namespace bf = boost::filesystem;



class DuplicateSearch {
private:
	enum class Hash {
		CRC32,
		MD5,
		NONE
	};

	unsigned long long size_block;
	char* buf;
	std::streampos current_pos = 0;
	Hash hash;
	std::unordered_map<std::string, std::vector<bf::path>> list;

	std::string GetHash(std::string& block) {
		std::string hash_transf;
		switch (hash)
		{
		case Hash::CRC32:
			hash_transf = CRC32_function((unsigned char*)block.c_str(), size_block);
			break;
		case Hash::MD5:
			hash_transf = md5(block);

			break;
		case Hash::NONE:
			break;
		default:
			break;
		}
		return hash_transf;
	}

	Hash HashType(std::string& type) noexcept {
		if (type == "MD5")
			return Hash::MD5;
		else if (type == "CRC32")
			return Hash::CRC32;
		else
			return Hash::NONE;
	}
public:
	DuplicateSearch(unsigned long long size, std::string& h) :size_block(size) {
		buf = new char[size_block];

		auto myToUpper = [](char ch) {return static_cast<char>(std::toupper(static_cast<unsigned char>(ch))); };
		std::transform(h.begin(), h.end(), h.begin(), myToUpper);
		hash = HashType(h);

	}

	unsigned long long CheckSizeBlock(std::ifstream& f) {
		f.seekg(current_pos, std::ios_base::beg);
		unsigned long long start = f.tellg();
		f.seekg(0, std::ios_base::end);
		unsigned long long end = f.tellg();
		auto remains = end - start;
		if (remains < size_block && remains>0) {
			return  remains;
		}
		return 0;
	}

	//std::string ReadBlock(bf::path& path) {
	//	std::ifstream file;
	//	file.open(path.string(), std::ios_base::binary);
	//	if (!file.is_open()) {
	//		std::cout << "File is not open!" << std::endl;
	//	}
	//	auto count_byte = CheckSizeBlock(file);
	//	//TODO Проверка выход на за пределы файла
	//	file.seekg(current_pos, std::ios_base::beg);
	//	file.read(buf, size_block);
	//	if (count_byte != 0) {
	//		for (auto i = count_byte; i != size_block; ++i) {
	//			buf[i] = '\0';
	//		}
	//	}
	//	//std::cout << count_byte << std::endl;
	//	//std::cout << std::string{ buf } << std::endl;

	//	file.close();
	//	return std::string{ buf };

	//}

	//void scanBlock(std::vector<bf::path>& list_path) {
	//	std::string temp_hash;
	//	for (auto& file : list_path) {
	//		temp_hash = ReadBlock(file);
	//		temp_hash = GetHash(temp_hash);
	//		if (list.find(temp_hash) != list.end())
	//			list[temp_hash].push_back(file);
	//		else
	//			list.insert(std::pair(temp_hash, std::vector{ file }));


	//	}
	//	current_pos += size_block;
	//	//print();
	//}

	std::tuple<std::string, std::string> ReadBlock(bf::path& first_path, bf::path& second_path)const {
		std::ifstream file1, file2;
		std::string first_file_block, second_file_block;

		file1.open(first_path.string(), std::ios_base::binary);
		file2.open(second_path.string(), std::ios_base::binary);
		if (!file1.is_open() || !file2.is_open()) {
			std::cout << "File is not open!" << std::endl;
		}

		auto count_byte = CheckSizeBlock(file1);
		file1.seekg(current_pos, std::ios_base::beg);
		file1.read(buf, size_block);
		if (count_byte != 0, count_byte != 0) {
			for (auto i = count_byte; i != size_block; ++i) {
				buf[i] = '\0';
			}
		}
		first_file_block = std::string{ buf };

		count_byte = CheckSizeBlock(file2);
		file2.seekg(current_pos, std::ios_base::beg);
		file2.read(buf, size_block);
		if (count_byte != 0) {
			for (auto i = count_byte; i != size_block; ++i) {
				buf[i] = '\0';
			}
		}
		second_file_block = std::string{ buf };
		//std::cout << count_byte << std::endl;
		//std::cout << std::string{ buf } << std::endl;

		file1.close();
		file2.close();
		return std::make_tuple{ first_file_block ,second_file_block };

	}

	bool checkHash(std::tuple<std::string, std::string>& p) {
		//TODO
		return true;
	}

	bool scanBlocks(bf::path& first, bf::path& second)const {
		std::tuple <std::string, std::string > blocks;
		blocks = ReadBlock(first, second);
		checkHash(blocks);
		if (list.find(temp_hash) != list.end())
			list[temp_hash].push_back(file);
		else
			list.insert(std::pair(temp_hash, std::vector{ file }));


	}
	current_pos += size_block;
	//print();
}

void searchDuplicate(std::vector<bf::path>& list_path, std::vector <std::vector <bf::path>>& list_duplicate) {
	for (auto file = list_path.cbegin(); file != list_path.cend() - 1; ++file) {
		scanBlocks(file, (file + 1));
	}

}

~DuplicateSearch() {
	delete[] buf;
}

void print() {
	for (auto h : list) {
		std::cout << h.first << "----" << std::endl;
		for (auto i = h.second.begin(); i != h.second.end(); ++i) {
			std::cout << *i << std::endl;
		}
		std::cout << "+++++" << std::endl;
	}
}

};