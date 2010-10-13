
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// Boost libraries
//#include <boost/program_options.hpp>
//namespace po = boost::program_options;


int main(int argc, char** argv)
{
	char exec_name[256];
	const char *const exec_args[] = {
		"Test",
	};
	const char *const exec_envargs[] = {
		"LD_PRELOAD=./gaCudaHook.so",
		NULL,
	};

	if (argc == 2) {
		strncpy(exec_name, argv[1], 256);
	} else {
		printf("Error: You need to specify a CUDA executable to run!\n");
		return -1;
	}

	execve(exec_name, (char* const*)exec_args, (char* const*)exec_envargs);
	
	/*
	// Handle the arguments
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "dump help")
		("test", po::value<int>(), "blaaah test")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		printf("%s\n", desc);
	}

	if (vm.count("test")) {
		printf("test was issued\n");
	}
  */
}

