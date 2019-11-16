
#include "WAVHandler.h"
#include "WAVFilter.cuh"
//Do audio operation corrosponding given argment and return result WAV
//Echo: DSP_CUDA input.wav (output.wav) --echo -> apply echo effect on input.wav and make result as 'output.wav'

Audio_WAV AudioHandler_WAV(Audio_WAV input, char* argv[], int argc, bool CUDAMode)
{
	if (argc == 2) // return as-is
		return input;
	if (argc == 3)
	{
		//TODO: check third text is optional argument
		
		//If not, just change name
		input.filename = argv[2];
		return input;
	}
	if (argc == 4)
	{
		//TODO: check third text is 'name'

	}
	//AS-IS
	return input;
}

