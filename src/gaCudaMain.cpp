
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ga.h"

//
// Append our LD_LIBRARY_PATH to the argument environment variables we
//  received, we need to do this because simply passing LD_PRELOAD in
//  will kill all the other environment variables to the executable we load.
char** appendEnv(char** arge, const char* newarg)
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
	char **exec_args = new char*[argc];
	
  // Copy argument list minus first element into exec_args
  for (int i = 0; i < argc - 1; i++) {
    exec_args[i] = new char[strlen(argv[i+1])];
    strcpy(exec_args[i], argv[i+1]);
  }
  exec_args[argc - 1] = NULL;

  // Copy environment variable list into arge, plus the LD_PRELOAD
	arge = appendEnv(arge, "LD_PRELOAD=./gaCudaHook.so");

	if (argc >= 2) {
		strncpy(exec_name, argv[1], 256);
	} else {
		printf("Error: You need to specify a CUDA executable to run!\n");
		return -1;
	}

  // Run the supplied CUDA application
	printf("Running executable: %s\n", exec_name);
	int error = execve(exec_name, (char* const*)exec_args, (char* const*)arge);
	
	if (error == -1) {
		printf("Error running executable: %s\n", strerror(errno));
	} else {
		printf("Executable run Ok!\n");
	}
	
}

