#define WRITE_CHUNK 1000

#include "WAVHandler.h"
#include "WAVFilter.cuh"

#include <iostream>
#include <fstream>  
//Do audio operation corrosponding given argment and return result WAV
//Echo: DSP_CUDA input.wav (output.wav) --echo -> apply echo effect on input.wav and make result as 'output.wav'

void AudioHandler_WAV(Audio_WAV& input, char* argv[], int argc, bool CUDAMode)
{
	std::cout << "Argc: " << argc << std::endl;
	if (argc == 2) // return as-is
		return;
	else if (argc == 3)
	{
		//TODO: check third text is optional argument
		FilterEchoCPU(input);

		//If not, just change name
		input.filename = argv[2];
	}
	else if (argc == 4)
	{
		//TODO: check third text is 'name'

	}
	else
		//AS-IS
		return;
}

void Create_WAVfile(Audio_WAV& result)
{
	std::ofstream outfile(result.filename + "_out.wav", std::ios::binary | std::ios::out);

	outfile.write((char *)&result.get_header(), 44);


	  char* res = result.get_audio();
	long size = result.get_header().Subchunk2Size;
	std::cout << size << std::endl;

	long i = 0;
	for (i = 0; i < size - WRITE_CHUNK; i += WRITE_CHUNK)
	{
		outfile.write(reinterpret_cast<char *>(&res[i]), WRITE_CHUNK);
	}
	outfile.write(reinterpret_cast<char*>(&res[i]), size % WRITE_CHUNK);

	outfile.close();
}