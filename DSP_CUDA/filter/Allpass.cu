#include "CUDAlib.cuh"
#include "WAVFilter.h"
#include <chrono>
#define BLOCK_SIZE 64

__global__ void FilterAllPassBlock(char* d_in, char* d_out, int offset, double gain)
{
	__shared__ int s_data[BLOCK_SIZE];
	int d_data;

	int dst_x = blockIdx.x * blockDim.x + threadIdx.x;
	int src_x = dst_x + offset;
	double t1 = (1 - (gain * gain));
	// Load one element per thread from device memory and store it 

	d_data = d_in[dst_x];
	if (dst_x >= offset)
		s_data[threadIdx.x] = d_in[src_x];
	else
		s_data[threadIdx.x] = 0;

	d_data += s_data[threadIdx.x] * gain;

	d_out[dst_x] = t1 * d_data - gain * s_data[threadIdx.x];
}
/*allpass came from 
* https://medium.com/the-seekers-project/coding-a-basic-reverb-algorithm-part-2-an-introduction-to-audio-programming-4db79dd4e325
* Written by Rishikesh Daoo 
*/

//RT for Reverbation Time
void FilterAllpass(Audio_WAV& origin, bool useCUDA, double delay, double gain, double RT) 
{
	WAV_HEADER origin_header = origin.get_header();
	size_t memSize = origin_header.Subchunk2Size;
	char* origin_bytes = origin.get_audio();
	int offset = origin_header.sampleRate * delay;

	if (useCUDA)
	{
		//pointer for device
		char* d_in, * d_out;

		int numBlocks = (memSize / BLOCK_SIZE) + 1; //celling
		int sharedMemSize = BLOCK_SIZE; //one byte for each thread

		cudaMalloc((void**)& d_in, memSize);
		cudaMalloc((void**)& d_out, memSize);

		cudaMemcpy(d_in, origin_bytes, memSize, cudaMemcpyHostToDevice);
		// launch kernel
		dim3 dimGrid(numBlocks);
		dim3 dimBlock(BLOCK_SIZE);
		FilterAllPassBlock << < dimGrid, dimBlock, sharedMemSize >> > (d_in, d_out, offset, gain);

		cudaThreadSynchronize();

		cudaMemcpy(origin_bytes, d_out, memSize, cudaMemcpyDeviceToHost);

		cudaFree(d_in);
		cudaFree(d_out);
	}
	else
	{
		char* delayed_sound = new char[memSize];
		double t1 = (1 - (gain * gain));
		std::memcpy((char*)delayed_sound, (char*)origin_bytes, memSize);

		for (int i = offset + 1; i < memSize; i++)
		{
			delayed_sound[i] += delayed_sound[i - offset] * gain;
		}
		for (int i = 0; i < memSize; i++)
		{
			origin_bytes[i] = -1 * gain * origin_bytes[i] + t1 * delayed_sound[i];
		}
	}
}
