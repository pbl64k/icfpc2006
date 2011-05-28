#include <iostream>
#include <ostream>
#include <string>

#include "um.hpp"

#include "prc.hpp"

#ifdef R_UM_OPT_SELFTEST
#include "tst.hpp"
#endif

using namespace Ranselett::Icfpc::UniversalMachine;

void displayUsage(const std::string &myself);

int main(int argc, char **argv) {
	std::string myself(argv[0]);

	if (argc != 2) {
		std::cout << "Incorrect number of command line arguments."
				<< std::endl;
		displayUsage(myself);
	} else {
		std::string arg(argv[1]);

#ifdef R_UM_OPT_SELFTEST
		if (! arg.compare("--selftest")) {
			TestSuite::run(myself);
		} else {
#endif
			if (! arg.compare("--sandmark")) arg = "sandmark.umz";
			if (! arg.compare("--stress")) arg = "stress.um";

			Processor *prc = new Processor(arg);
			prc->run();
			delete prc;
#ifdef R_UM_OPT_SELFTEST
		}
#endif
	}
}

void displayUsage(const std::string &myself) {
	std::cout << std::endl;
	std::cout << "Usage:" << std::endl;
#ifdef R_UM_OPT_SELFTEST
	std::cout << "\t" << myself << " --selftest" << std::endl;
	std::cout << "\tOR" << std::endl;
#endif
	std::cout << "\t" << myself << " --sandmark" << std::endl;
	std::cout << "\tOR" << std::endl;
	std::cout << "\t" << myself << " [filename]" << std::endl;
	std::cout << std::endl;
}
