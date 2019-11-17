#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <chrono>

#include "CUDAlib.cuh"
#include "AudioEntry.h"
#include "WAVHandler.h"
#include "utils/Gadgets.h"
/*
argument list:
-echo x y : apply echo effect x times with delay of y seconds
*/

int main(int argc, char* argv[])
{
	std::ifstream audiofile;
	std::string filename;
	audio_type filetype;

	//Manual input mode
	if (argc < 3)
	{
		std::cout << "Type audio file path (WAV) : ";
		std::cin >> filename;	
	}
	else filename = argv[1];

	//Open file
	audiofile.open(filename, std::ios::binary | std::ios::in);
	
	if (!audiofile)
	{	
		std::cout << "Something went wrong while opening file \"" << filename << "\"" << std::endl;
		std::cout << "Aborting...." << std::endl;
		return;
	}
	cudaFree(0);
	//after opening file, determine given file's type 
	//일단은 wav만 한다고 가정
	std::string expender = find_expender(filename);
	if (expender == "wav")
	{
		//Create WAV object
		Audio_WAV item(audiofile,filename);

		// Record start time
		auto start = std::chrono::high_resolution_clock::now();

		//Processing WAV object(With CUDA)
		AudioHandler_WAV(item, argv, argc, true);

		// Record end time
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		std::cout << "Elapsed time(GPU): " << elapsed.count() * 1000 << " ms\n";

		// Record start time
		start = std::chrono::high_resolution_clock::now();

		//Processing WAV object(Without CUDA)
		AudioHandler_WAV(item, argv, argc, false);

		// Record end time
		finish = std::chrono::high_resolution_clock::now();
		elapsed = finish - start;
		std::cout << "Elapsed time(CPU): " << elapsed.count() * 1000 << " ms\n";

		//print WAV object
		Create_WAVfile(item);
	}
	else
	{
		std::cout << "only WAV file is supported...aborting." << std::endl;
	}
}

