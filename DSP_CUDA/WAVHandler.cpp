
#include "WAVHandler.h"
#include "WAVFilter.cuh"

#include <iostream>
#include <fstream>  
//Do audio operation corrosponding given argment and return result WAV
//Echo: DSP_CUDA input.wav (output.wav) --echo -> apply echo effect on input.wav and make result as 'output.wav'

Audio_WAV AudioHandler_WAV(Audio_WAV input, char* argv[], int argc, bool CUDAMode)
{
	std::cout << "Argc: " << argc << std::endl;
	if (argc == 2) // return as-is
		return input;
	else if (argc == 3)
	{
		//TODO: check third text is optional argument
		return FilterEchoCPU(input);

		//If not, just change name
		input.filename = argv[2];
		return input;
	}
	else if (argc == 4)
	{
		//TODO: check third text is 'name'

	}
	else
	//AS-IS
	return input;
}

void Create_WAVfile(Audio_WAV result)
{
	std::cout << "Creating " << result.filename << std::endl;
	std::ofstream outfile(result.filename);

	outfile.write((char *)&result.get_header(), 44);
	outfile.write((char *)result.get_audio(), result.get_header().Subchunk2Size);

	outfile.close();
}