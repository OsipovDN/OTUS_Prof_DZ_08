#include "bayan.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp> 
#include <iostream>
#include <vector>

namespace bf = boost::filesystem;
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	po::options_description desc;
	desc.add_options()
		("help,h", "produce help")
		("optimization,o", po::value<std::vector<bf::path>>()->composing()->multitoken(), "optimization level");

	po::variables_map vm;
	po::store(parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
		std::cout << desc << '\n';
	else if (vm.count("optimization")) {
		
		for (auto it : vm["optimization"].as<std::vector<bf::path>>()) {
			std::cout << "readfrom: " << it << std::endl;
		}
	}
		

}
