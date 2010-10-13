
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Boost libraries
//#include <boost/program_options.hpp>
//namespace po = boost::program_options;


//
// Append our LD_LIBRARY_PATH to the argument environment variables we
//  received, we need to do this because simply passing LD_PRELOAD in
//  will kill all the other environment variables to the executable we load.
char** appendEnv(char** arge, char* newarg)
{
	int argec = 1;
	char **retVal = NULL;

	// Find the number of args
	while (arge[argec++]);

	// Create new memory for the appended argument list
	retVal = new char*[argec];
	for (int i = 0; i < argec - 1; i++) {
		// Copy the existing env vars
		retVal[i] = new char[strlen(arge[i]) + 1];
		strcpy(retVal[i], arge[i]);
	}

	// Alloc and copy the new var
	retVal[argec-1] = new char[strlen(newarg) + 1];
	strcpy(retVal[argec-1], newarg);

	retVal[argec] = NULL;

	return retVal;
}


//
// Entry point
int main(int argc, char** argv, char** arge)
{
	char exec_name[256];
	const char *const exec_args[] = {
		NULL,
	};
	const char *const exec_envargs[] = {
		"LD_PRELOAD=./gaCudaHook.so",
		"LD_LIBRARY_PATH=/usr/local/cuda/lib64",
		NULL,
	};

	
	char *retval = getenv("LD_LIBRARY_PATH");
	printf("env: %s\n", retval);

	arge = appendEnv(arge, "LD_PRELOAD=./gaCudaHook.so");

	printf("Env vars:\n");
	char *itr = NULL;
	int i = 0;
	do {
		itr = arge[i++];
		printf("   Var: %s\n", itr);
	} while (itr);
	


	if (argc == 2) {
		strncpy(exec_name, argv[1], 256);
	} else {
		printf("Error: You need to specify a CUDA executable to run!\n");
		return -1;
	}

	printf("Running executable: %s\n", exec_name);
	int error = execve(exec_name, (char* const*)exec_args, (char* const*)exec_envargs);
	
	if (error == -1) {
		printf("Error running executable: %s\n", strerror(errno));
	} else {
		printf("Executable run Ok!\n");
	}
	
}

