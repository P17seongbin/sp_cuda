#include "CUDAlib.cuh"
#include "WAVFilter.h"
#include <chrono>
#define BLOCK_SIZE 64

// reduce bit value to half means -6dB

__global__ void FilterCompressorBlock(char* d_in, char* d_out, double threshold, double ratio, int bitperSample)
{
	double max_dB = 6.02 * bitperSample; // approx. value of SNR. https://en.wikipedia.org/wiki/Audio_bit_depth#Bit_rate_and_file_size

	int byterate = bitperSample / 8;
	short d_data = 0;
	int x = (blockIdx.x * blockDim.x + threadIdx.x) * byterate;
	// Load one element per thread from device memory and store it 

	memcpy(&d_data, (d_in + x), byterate);

	double dB;

	if (d_data < 0)
		dB = max_dB + 10 * log10((double)(-1 * d_data)) - 3.012 * bitperSample;
	else
		dB = max_dB + 10 * log10((double)(d_data)) - 3.012 * bitperSample;

	if (dB > threshold)
	{
		dB = ratio * (dB - threshold) + threshold;
		dB += (3.012 * bitperSample - max_dB);
		dB /= 10;
		short v = (short)pow(10.0, dB);
		d_data = (d_data < 0) ? (-1 * v) : v;
	}

	memcpy((d_out + x), &d_data, byterate);
	
}

void FilterCompressor(Audio_WAV& origin, bool useCUDA, double threshold, double ratio)
{
	WAV_HEADER origin_header = origin.get_header();
	size_t memSize = origin_header.Subchunk2Size;
	char* origin_bytes = origin.get_audio();
	int bitperSample = origin_header.bitsPerSample;
	int byterate = bitperSample / 8;

	if (useCUDA)
	{
		//pointer for device
		char* d_in, * d_out;

		int numBlocks = (memSize / BLOCK_SIZE) + 1; //celling
		int sharedMemSize = BLOCK_SIZE * byterate; //number byte for each thread

		cudaMalloc((void**)& d_in, memSize);
		cudaMalloc((void**)& d_out, memSize);

		cudaMemcpy(d_in, origin_bytes, memSize, cudaMemcpyHostToDevice);
		// launch kernel
		dim3 dimGrid(numBlocks);
		dim3 dimBlock(BLOCK_SIZE / byterate);

		FilterCompressorBlock << < dimGrid, dimBlock, sharedMemSize >> > (d_in, d_out, threshold, ratio, bitperSample);

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

		double max_dB = 6.02 * bitperSample; // approx. value of SNR. https://en.wikipedia.org/wiki/Audio_bit_depth#Bit_rate_and_file_size
		for (int i = 0; i < memSize; i += byterate)
		{
			short value;
			memcpy(&value, &origin_bytes[i], byterate);
			double dB;

			if (value < 0)
				dB = max_dB + 10 * log10((double)(-1 * value)) - 3.012 * bitperSample;
			else
				dB = max_dB + 10 * log10((double)(value)) - 3.012 * bitperSample;

			if (dB > threshold)
			{
				dB = ratio * (dB - threshold) + threshold;
				dB += (3.012 * bitperSample - max_dB);
				dB /= 10;
				short v = (short)pow(10, dB);
				value = (value < 0) ? (-1 * v) : v;
			}
			memcpy(&origin_bytes[i], &value, byterate);
		}
	}
}
