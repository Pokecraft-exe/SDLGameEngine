#pragma once
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cstdarg>
#include "maths_engine.h"

typedef __global__ void GPU_CALL(__parameters);

class GPU
{
private:
	int device = 0;
public:
	GPU(int device);
	cudaError_t VerifyCuda();
	cudaError_t Thread(GPU_CALL* func, __parameters params);
};
