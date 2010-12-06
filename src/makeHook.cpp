
#include <dlfcn.h>

#include <stdio.h>

static void init() __attribute__((constructor));
static void shutdown() __attribute__((destructor));

typedef FILE* (*fopen64Type)(const char*, const char*);
typedef FILE* (*fopenType)(const char*, const char*);
typedef int (*openType)(const char *, int);

void init()
{
	printf("makeHook library loaded.\n");
}

void shutdown()
{
	printf("makeHook library unloaded.\n");
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
  */

  void copyFile(const char* filename, const char* dst_filename)
  {
    FILE* fp = fopen(filename, "r");
    int filesize = 0;

    if (fp) {
      fseek(fp, 0, SEEK_END);
      filesize = ftell(fp);
      fseek(fp, 0, SEEK_SET);

      char *buf = new char[filesize];
      fread(buf, 1, filesize, fp);
      fclose(fp);

      FILE* dst = fopen(dst_filename, "w+");
      if (dst) {
        fwrite(buf, 1, filesize, fp);
        fclose(dst);
      } else {
        printf("Error: Could not open destination file for writing: %s\n", dst_filename);
        return;
      }

      delete[] buf;
    }
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

    const char dst[] = "dst.cu";      
    printf("open: %s %s %i\n", pathname, dst, oflag);
   
    //return realopen(pathname, oflag); 
    return realopen(dst, oflag); 
  }  
}

