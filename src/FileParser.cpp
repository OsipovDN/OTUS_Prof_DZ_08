#include "FileParser.h"
namespace bf = boost::filesystem;
namespace ba = boost::adaptors;


FileParser::FileParser(int l, unsigned long long mfs, std::string m)
	:level(l), min_file_size(mfs), fact_lvl_dir(0) {
	mask = std::make_unique<boost::regex>(m);
}

void FileParser::ScanDir(const bf::path& in_dir, const std::vector < bf::path>& ex_dir, std::vector < bf::path>& list_file) {

	auto filterPath = [&](const bf::path& p)->bool {return std::find(ex_dir.begin(), ex_dir.end(), p) == std::end(ex_dir) ? true : false; };

	if (fact_lvl_dir <= level) {
		for (const auto& it : boost::make_iterator_range(bf::directory_iterator{ in_dir }, bf::directory_iterator{})
			| ba::filtered(filterPath)) {
			if (bf::is_directory(it.path())) {
				fact_lvl_dir++;
				ScanDir(it.path(), ex_dir, list_file);
				fact_lvl_dir--;
			}
			else
				list_file.push_back(it.path());
		}
	}
}

std::vector <bf::path> FileParser::ScanListDir(std::vector<bf::path>& list_dir, const std::vector<bf::path>& ex_dir) {
	std::vector < bf::path> file_list;
	for (const auto& dir : list_dir) {
		fact_lvl_dir = 0;
		ScanDir(dir, ex_dir, file_list);
	}
	return file_list;
}

void FileParser::FilterListFile(std::vector <bf::path>& file_list) {
	std::vector <bf::path> temp_list;
	auto FilterSize = [&](const bf::path& p)->bool {return file_size(p) > min_file_size; };
	auto FilterMask = [&](const bf::path& p)->bool {return !boost::regex_match(p.filename().string(), *mask); };

	for (const auto& it : file_list
		| ba::filtered(FilterSize)
		| ba::filtered(FilterMask)) {
		temp_list.push_back(it);
	}
	file_list =temp_list;

}



