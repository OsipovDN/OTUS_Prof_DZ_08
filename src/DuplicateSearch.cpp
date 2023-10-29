#include "DuplicateSearch.h"

DuplicateSearch::DuplicateSearch(unsigned long long size, std::string& h) :size_block(size) {
	buf = new char[size_block];
	auto myToUpper = [](char ch) {return static_cast<char>(std::toupper(static_cast<unsigned char>(ch))); };
	std::transform(h.begin(), h.end(), h.begin(), myToUpper);
	hash = hashType(h);
}
std::string DuplicateSearch::getHash(std::string& block) {
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
DuplicateSearch::Hash DuplicateSearch::hashType(std::string& type) noexcept {
	if (type == "MD5")
		return Hash::MD5;
	else if (type == "CRC32")
		return Hash::CRC32;
	else
		return Hash::NONE;
}
unsigned long long DuplicateSearch::checkSizeBlock(std::ifstream& f) {
	f.seekg(current_pos, std::ios_base::beg);
	unsigned long long start = f.tellg();
	f.seekg(0, std::ios_base::end);
	unsigned long long end = f.tellg();
	auto remains = end - start;
	return  remains;
}
std::string DuplicateSearch::readBlock(bf::path& path) {
	std::ifstream file;
	file.open(path.string(), std::ios_base::binary);
	if (!file.is_open()) {
		std::cout << "File is not open!" << std::endl;
	}
	auto count_byte = checkSizeBlock(file);
	if (count_byte < size_block || count_byte == 0) {
		for (auto i = count_byte; i != size_block; ++i)
			buf[i] = '\0';
	}
	file.seekg(current_pos, std::ios_base::beg);
	file.read(buf, size_block);
	//std::cout << count_byte << std::endl;
	//std::cout << std::string{ buf } << std::endl;
	file.close();
	return std::string{ buf };
}

void DuplicateSearch::scanBlock(std::vector<bf::path>& list_path) {
	std::string temp_hash;
	for (auto& file : list_path) {
		temp_hash = readBlock(file);
		temp_hash = getHash(temp_hash);

	}
	current_pos += size_block;
	//print();
}
void DuplicateSearch::checkHashInList(std::string& h, bf::path& path) {

	if (list.find(h) != list.end())
		list[h].push_back(path);
	else
		list.insert(std::pair(h, std::vector{ path }));

}
void DuplicateSearch::searchDuplicate(std::vector<bf::path>& list_path, std::vector <std::vector <bf::path>>&) {
	std::string block;
	std::string h;

	for (auto file : list_path) {
		block = readBlock(file);
		h = getHash(block);
		checkHashInList(h, file);
	}
	print();
}
void DuplicateSearch::print() {
	for (auto const& [key, value] : list) {
		std::cout << key << "----" << std::endl;
		for (auto i = value.begin(); i != value.end(); ++i) {
			std::cout << *i << std::endl;
		}
		std::cout << "+++++" << std::endl;
	}
}
DuplicateSearch::~DuplicateSearch() {
	delete[] buf;
}
