#include "CUDAlib.cuh"
#include "WAVFilter.h"
#include <chrono>
#define BLOCK_SIZE 64

__global__ void FilterEchoBlock(char* d_in, char* d_out, long offset, double gain)
{
	__shared__ int s_data[BLOCK_SIZE];
	char d_data;

	int dst_x = blockIdx.x * blockDim.x + threadIdx.x;
	int src_x = dst_x - offset;
	// Load one element per thread from device memory and store it 

	d_data = d_in[dst_x];

	if (dst_x > offset)
		s_data[threadIdx.x] = *(d_in+src_x);
	else
		s_data[threadIdx.x] = 0;

	d_data += s_data[threadIdx.x] * gain;
	d_out[dst_x] = d_data;
}

void FilterEcho(Audio_WAV& origin, bool useCUDA, double delay, double gain)
{
	WAV_HEADER origin_header = origin.get_header();
	size_t memSize  = origin_header.Subchunk2Size;
	char* origin_bytes = origin.get_audio();
	unsigned long byteperSecond = origin_header.sampleRate * origin_header.blockAlign;
	long offset = byteperSecond * delay;

	if (useCUDA)
	{
		//pointer for device
		char *d_in, *d_out;

		int numBlocks = (memSize / BLOCK_SIZE) + 1; //celling
		int sharedMemSize = BLOCK_SIZE; //one byte for each thread

		cudaMalloc(&d_in, memSize);
		cudaMalloc(&d_out, memSize);

		cudaMemcpy(d_in, origin_bytes, memSize, cudaMemcpyHostToDevice);
		// launch kernel
		dim3 dimGrid(numBlocks);
		dim3 dimBlock(BLOCK_SIZE);
		FilterEchoBlock << < dimGrid, dimBlock, sharedMemSize >> > (d_in, d_out, offset, gain);

		cudaThreadSynchronize();

		memset(origin_bytes, 0, memSize);

		char* origin_archive = new char[memSize];
		cudaMemcpy(origin_archive, d_out, memSize, cudaMemcpyDeviceToHost);

		origin.set_audio(origin_archive);

		cudaFree(d_in);
		cudaFree(d_out);
	}
	else
	{
		char* origin_archive = new char[memSize];
		std::memcpy((char*)origin_archive, (char*)origin_bytes, memSize);

		for (int i = offset + 1; i < memSize; i++)
		{
			origin_bytes[i] = origin_archive[i] + origin_archive[i - offset] * gain;
		}
	}
}
