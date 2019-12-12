#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <chrono>

#include "CUDAlib.cuh"
#include "AudioEntry.h"
#include "WAVHandler.h"
#include "utils/Gadgets.h"

#include <time.h>
/*
argument list:
-echo x y : apply echo effect x times with delay of y seconds
*/

int main(int argc, char* argv[])
{
	std::ifstream audiofile;
	std::string filename;
	std::chrono::system_clock::time_point start, end;
	audio_type filetype;

	//Manual input mode
	if (argc < 3)
	{
		std::cout << "Type audio file path : ";
		std::cin >> filename;	
	}
	//Argument input mode
	else filename = argv[1];

	audiofile.open(filename, std::ios::binary | std::ios::in);
	
	//If fail to open given audio file
	if (!audiofile)
	{	
		std::cout << "Something went wrong while opening file \"" << filename << "\"" << std::endl;
		std::cout << "Aborting...." << std::endl;
		return;
	}

	//Init CUDA system
	cudaFree(0);
	std::string expender = find_expender(filename);
	//this program only supports .WAV file.
	if (expender == "wav")
	{
		//Create WAV object
		Audio_WAV item(audiofile,filename);

		start = std::chrono::system_clock::now();
		//Processing WAV object(With CUDA if available)
		AudioHandler_WAV(item, argv, argc, true);
		end  = std::chrono::system_clock::now();

		std::cout << "GPU: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0 << " ms" << std::endl;

		start = std::chrono::system_clock::now();
		//Processing WAV object(Without CUDA)
		AudioHandler_WAV(item, argv, argc, false);

		end = std::chrono::system_clock::now();
		std::cout << "CPU: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0 << " ms" << std::endl;

		//print WAV object
		Create_WAVfile(item);
		std::cout << "WAV file build complete" << std::endl;
	}
	else
	{
		std::cout << "only WAV file is supported...aborting." << std::endl;
	}
}

