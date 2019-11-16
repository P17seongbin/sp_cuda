#include "CUDAlib.cuh"
#include "Filter.cuh"
#include "FileHandler.h"

#include <string>
#include <iostream>

/*
argument list:
-echo x y : apply echo effect x times with delay of y seconds
*/
int main(int argc, char* argv[])
{
	//일단은 argument없이 구현
	FILE *audiofile = NULL;
	audio_type filetype;

	std::string filename; 

	//Manual input mode
	if (argc < 3)
	{
		std::cout << "Type audio file path (WAV, MPC, FLAC) : ";
		std::cin >> filename;	
	}
	else
	{
		filename = argv[1];
	}
	if ((audiofile = open_audio(filename, &filetype)) == NULL)
	{ 
		std::cout << "Something went wrong while opening file \"" << filename << "\"" << std::endl;
		std::cout << "Aborting...." << std::endl;
	}
	
}