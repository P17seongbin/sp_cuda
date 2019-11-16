constexpr auto WRITE_CHUNK = 1000;

#include "WAVHandler.h"
#include "WAVFilter.h"
#include "utils/Gadgets.h"

#include <iostream>
#include <fstream>  
//Do audio operation corrosponding given argment and return result WAV
//Echo: DSP_CUDA input.wav (output.wav) --echo -> apply echo effect on input.wav and make result as 'output.wav'

void AudioHandler_WAV(Audio_WAV& input, char* argv[], int argc, bool TryCUDA)
{
	int CUDAcount;
	cudaGetDeviceCount(&CUDAcount);

	bool CUDAMode = TryCUDA && CUDAcount > 0;

	std::cout << "Argc: " << argc << std::endl;
	if (argc == 2) // return as-is
		return;
	else if (argc == 3)
	{
		std::string argv_2(argv[2]);
		if (find_expender(argv_2) != "wav")
		{
			if (argv_2 == "--echo")
				if (CUDAMode)
					FilterEchoCUDA(input);
				else
					FilterEchoCPU(input);
		}
		else
		{
			//If not, just change name
			input.filename = argv[2];
		}
	}
	else if (argc == 4)
	{
		//TODO: check third text is 'name'
		std::string argv_2(argv[2]);
		std::string argv_3(argv[3]);
		//we only need to check "WAV" cause this is a WAV handler
		if (find_expender(argv_2) == "wav")
		{
			input.filename = argv[2];
			if (argv_3 == "--echo")
				if (CUDAMode)
					FilterEchoCUDA(input);
				else
					FilterEchoCPU(input);
		}
		else
		{
			std::cout << "Invalid arguments: " << argv_2 << std::endl;
			exit(0);
		}
	}
}

void Create_WAVfile(Audio_WAV& result)
{
	std::ofstream outfile(result.filename, std::ios::binary | std::ios::out);

	outfile.write((char *)&result.get_header(), 44);

	char* res = result.get_audio();
	long size = result.get_header().Subchunk2Size;

	long i = 0;
	for (i = 0; i < size - WRITE_CHUNK; i += WRITE_CHUNK)
	{
		outfile.write(&res[i], WRITE_CHUNK);
	}
	outfile.write(&res[i], size % WRITE_CHUNK);

	outfile.close();
}