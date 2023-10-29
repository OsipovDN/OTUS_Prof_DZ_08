#pragma once
//stl
#include <fstream>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <string>
#include <algorithm>
//Hash
#include "md5.h"
#include "crc32.h"
//Boost
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
	std::streampos current_pos = 0;
	char* buf;
	Hash hash;
	std::unordered_map<std::string, std::vector<bf::path>> list;

	//������������ ���� ���������� �����
	std::string getHash(std::string& block);
	//�������� ���� hash-�������
	Hash hashType(std::string& type) noexcept;
	//��������� ����������� ���������� ����
	unsigned long long checkSizeBlock(std::ifstream& f);
public:
	DuplicateSearch(unsigned long long size, std::string& h);

	std::string readBlock(bf::path& path);
	void scanBlock(std::vector<bf::path>& list_path);

	void checkHashInList(std::string& h, bf::path& path);

	//�������� ������� ������ ����������
	void searchDuplicate(std::vector<bf::path>& list_path, std::vector <std::vector <bf::path>>&);

	//����� �� ����� ������ ����������
	void print();
	~DuplicateSearch();



};