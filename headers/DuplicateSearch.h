#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>

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

	std::string ReadBlock(bf::path& path) {
		std::ifstream file;
		file.open(path.string(), std::ios_base::binary);
		if (!file.is_open()) {
			std::cout << "File is not open!" << std::endl;
		}
		auto count_byte = CheckSizeBlock(file);

		file.seekg(current_pos, std::ios_base::beg);
		file.read(buf, size_block);
		if (count_byte != 0) {
			for (auto i = count_byte; i != size_block; ++i) {
				buf[i] = '\0';
			}
		}
		//std::cout << count_byte << std::endl;
		//std::cout << std::string{ buf } << std::endl;

		file.close();
		return std::string{ buf };

	}

	void search(std::vector<bf::path>& list_path) {
		std::string temp_hash;
		for (auto& file : list_path) {
			temp_hash = ReadBlock(file);
			temp_hash = GetHash(temp_hash);
			if (list.find(temp_hash) != list.end())
				list[temp_hash].push_back(file);
			else
				list.insert(std::pair(temp_hash, std::vector{ file }));


		}
		current_pos += size_block;
		//print();
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