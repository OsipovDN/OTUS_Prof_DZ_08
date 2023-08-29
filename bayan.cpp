#include "bayan.h"
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

void set_bulk(size_t bulk) {
    std::cout << "bulk size is " << bulk << std::endl;
}

int main(int argc, const char* argv[]) {
    try {
        po::options_description desc{ "Options" };
        desc.add_options()
            ("help,h", "This screen")
            ("config", po::value<std::string>()->default_value("app.yaml"), "config filename")
            ("bulk", po::value<size_t>()->default_value(5)->notifier(set_bulk), "bulk size");

        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
            std::cout << desc << '\n';
        else if (vm.count("config"))
            std::cout << "readfrom: " << vm["config"].as<std::string>() << std::endl;
        else if (vm.count("bulk"))
            std::cout << "bulk: " << vm["bulk"].as<size_t>() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
