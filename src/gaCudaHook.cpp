
#include <dlfcn.h>

#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime_api.h>

static void init(void) __attribute__((constructor));
static void shutdown(void) __attribute__((destructor));

typedef cudaError_t(*cudaConfigureType)(dim3, dim3, size_t, cudaStream_t);


extern "C"
{
  cudaError_t cudaConfigureCall(dim3 gridDim, dim3 blockDim, size_t sharedMem, cudaStream_t stream)
  {
    static cudaConfigureType realCudaConfigure = NULL;

    if (!realCudaConfigure) {
      void *handle = dlopen("/usr/local/cuda/lib/libcudart.so", RTLD_LAZY);

      if (!handle) {
        printf("Error opening libcudart.so\n");
        return cudaSuccess;
      }

      //realCudaConfigure = (cudaConfigureType)dlsym(RTLD_NEXT, "cudaConfigure");
      realCudaConfigure = (cudaConfigureType)dlsym(handle, "cudaConfigureCall");

      if (dlerror() != NULL) {
        printf("Error: Could not retrieve real cudaConfigure function!\n");
        return cudaSuccess;
      }
    }

    printf("Real: %p\n", realCudaConfigure);
    printf("cudaConfigureCall\n");
    printf("  gridDim: %i %i %i\n", gridDim.x, gridDim.y, gridDim.z);
    printf("  blockDim: %i %i %i\n", blockDim.x, blockDim.y, blockDim.z);
    printf("  sharedMem: %li\n", sharedMem);

    realCudaConfigure(gridDim, blockDim, sharedMem, stream);
  }


}



