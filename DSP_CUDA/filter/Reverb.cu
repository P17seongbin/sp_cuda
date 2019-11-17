#include "CUDAlib.cuh"
#include "WAVFilter.h"
#include <chrono>
#define BLOCK_SIZE 64

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
		FilterEchoBlock << < dimGrid, dimBlock, sharedMemSize >> > (d_in, d_out, offset, gain);

		cudaThreadSynchronize();

		cudaMemcpy(origin_bytes, d_out, memSize, cudaMemcpyDeviceToHost);

		cudaFree(d_in);
		cudaFree(d_out);
	}
	else
	{
		char* delayed_sound = new char[memSize];
		std::memcpy((char*)delayed_sound, (char*)origin_bytes, memSize);

		for (int i = 0; i < memSize; i++)
		{
			if(i > offset)
				delayed_sound[i] =  delayed_sound[i - offset] * gain;
		}

		
	}
}
