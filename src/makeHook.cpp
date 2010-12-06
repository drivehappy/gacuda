
#include <dlfcn.h>

#include <stdio.h>
#include <string>
using namespace std;

static void init() __attribute__((constructor));
static void shutdown() __attribute__((destructor));

typedef FILE* (*fopen64Type)(const char*, const char*);
typedef FILE* (*fopenType)(const char*, const char*);
typedef int (*openType)(const char *, int);

void init()
{
	//printf("makeHook library loaded.\n");
}

void shutdown()
{
	//printf("makeHook library unloaded.\n");
}

extern "C"
{
  /* make
  FILE *fopen64(const char *filename, const char *type)
  {
    static fopen64Type realfopen64 = NULL;

    if (!realfopen64) {
      void *handle = dlopen("/usr/lib/libstdc++.so.6", RTLD_LAZY);

      if (!handle) {
        printf("Error opening libstdc++.so.6\n");
        return NULL;
      }

      realfopen64 = (fopen64Type)dlsym(handle, "fopen64");

      if (dlerror() != NULL) {
        printf("Error: Could not retrieve real fopen64 function!\n");
        return NULL;
      }
    }
      
    printf("fopen64: %s %s\n", filename, type);
   
    return realfopen64(filename, type); 
  }  
  */

  FILE *fopen(const char *filename, const char *type)
  {
    static fopenType realfopen = NULL;

    if (!realfopen) {
      void *handle = dlopen("/usr/lib/libstdc++.so.6", RTLD_LAZY);

      if (!handle) {
        printf("Error opening libstdc++.so.6\n");
        return NULL;
      }

      realfopen = (fopenType)dlsym(handle, "fopen");

      if (dlerror() != NULL) {
        printf("Error: Could not retrieve real fopen function!\n");
        return NULL;
      }
    }
      
    printf("fopen: %s %s\n", filename, type);
   
    return realfopen(filename, type); 
  }  
  


  // g++
  int open(const char *pathname, int oflag)
  {
    static openType realopen = NULL;

    if (!realopen) {
      void *handle = dlopen("/usr/lib/libstdc++.so.6", RTLD_LAZY);

      if (!handle) {
        printf("Error opening libstdc++.so.6\n");
        return NULL;
      }

      realopen = (openType)dlsym(handle, "open");

      if (dlerror() != NULL) {
        printf("Error: Could not retrieve real open function!\n");
        return NULL;
      }
    }

    string filename = extractFilename(pathname);
    string noextFilename = extractNoExtension(filename);
    string extension = extractExtension(pathname);
    string path = extractPathname(pathname);

    //printf("open: %s %i\n", pathname, oflag);
    /*
    printf("  filename: %s\n", filename.c_str());
    printf("  path: %s\n", extractPathname(pathname).c_str());
    printf("  extension: %s\n", extension.c_str());
    printf("  no_extension: %s\n", noextFilename.c_str());
    */

    if (extension.compare(string("cu")) == 0) {
      string dstString = path + string("/") + noextFilename + string(".") + extension + string(".lock");
      printf("  copy: %s\n", dstString.c_str());
 
      // CUDA file, copy it
      copyFile(pathname, dstString.c_str());
      return realopen(dstString.c_str(), oflag); 
    } else {
      // Regular file, let it through
      return realopen(pathname, oflag); 
    }
  }  
}

