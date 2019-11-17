#include "CUDAlib.cuh"
#include "WAVFilter.h"
#include <chrono>
#define BLOCK_SIZE 64

__global__ void FilterEchoBlock(char* d_in, char* d_out, int offset)
{
	__shared__ int s_data[BLOCK_SIZE];
	int d_data;

	int dst_x = blockIdx.x * blockDim.x + threadIdx.x;
	int src_x = dst_x + offset;
	// Load one element per thread from device memory and store it 
	// *in reversed order* into temporary shared memory

	d_data = d_in[dst_x];
	if (dst_x >= offset)
		s_data[threadIdx.x] = d_in[src_x];
	else
		s_data[threadIdx.x] = 0;

	// Block until all threads in the block have written their data to shared mem

	__syncthreads();

	d_data += s_data[threadIdx.x];
	d_out[dst_x] = d_data;
}

void FilterEchoCUDA(Audio_WAV& origin)
{
	WAV_HEADER origin_header = origin.get_header();
	size_t memSize  = origin_header.Subchunk2Size;
	char* origin_bytes = origin.get_audio();
	int offset = origin_header.sampleRate / 4;

	//pointer for device
	char *d_in, * d_out;

	int numBlocks = (memSize / BLOCK_SIZE) + 1; //celling
	int sharedMemSize = BLOCK_SIZE; //one byte for each thread

	cudaMalloc((void**) &d_in, memSize);
	cudaMalloc((void**) &d_out, memSize);

	cudaMemcpy(d_in, origin_bytes, memSize, cudaMemcpyHostToDevice);
	// launch kernel
	dim3 dimGrid(numBlocks);
	dim3 dimBlock(BLOCK_SIZE);
	FilterEchoBlock << < dimGrid, dimBlock, sharedMemSize >> > (d_in, d_out, offset);


	cudaThreadSynchronize();

	cudaMemcpy(origin_bytes, d_out, memSize, cudaMemcpyDeviceToHost);


	cudaFree(d_in);
	cudaFree(d_out);

}
