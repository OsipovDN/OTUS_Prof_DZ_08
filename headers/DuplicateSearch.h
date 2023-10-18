#pragma once
#include <fstream>
#include <memory>
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
	char* buf;
	std::streampos current_pos = 0;
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
		buf = new char[size_block];
		boost::algorithm::to_upper(hash);
		h = HashType(hash);
	}


	void ReadFile(std::string& path_to_file) {
		std::ifstream file;
		while (!file.eof()) {
			file.open(path_to_file, std::ios_base::binary);
			if (!file.is_open()) {
				std::cout << "File is not open!" << std::endl;
			}
			file.seekg(current_pos, std::ios_base::beg);
			file.read(buf, static_cast<std::streamsize>(size_block));

			current_pos = file.tellg();
			file.close();
		}

	}

	~DuplicateSearch() {
		delete[] buf;
	}


};