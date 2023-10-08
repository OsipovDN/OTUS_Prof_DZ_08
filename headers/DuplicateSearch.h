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
};