#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include "CUDAlib.cuh"
#include "Filter.cuh"
#include "AudioEntry.h"
#include "WAVHandler.h"
/*
argument list:
-echo x y : apply echo effect x times with delay of y seconds
*/
int main(int argc, char* argv[])
{

	//일단은 argument없이 구현
	std::ifstream audiofile;
	audio_type filetype;

	std::string filename; 
	std::cout << argc << std::endl;
	//Manual input mode
	if (argc < 3)
	{
		std::cout << "Type audio file path (WAV) : ";
		std::cin >> filename;	
	}
	else filename = argv[1];
	//Open file
	audiofile.open(filename, std::ios::binary | std::ios::in );
	
	if (!audiofile)
	{	
		std::cout << "Something went wrong while opening file \"" << filename << "\"" << std::endl;
		std::cout << "Aborting...." << std::endl;
		return;
	}
	
	//after opening file, determine given file's type 
	//do something 
	//일단은 wav만 한다고 가정
	std::string token;
	std::string t_name = filename;
	std::stringstream ss(t_name);

	while (std::getline(ss, token, '.'));
	if (token == "wav")
	{		

		//Create WAV object
		Audio_WAV i(audiofile,filename);
		//Processing WAV object(With CUDA)
		//print WAV object
		AudioHandler_WAV(i, argv, argc, true);
		Create_WAVfile(i);
	}
	else
	{
		std::cout << "only WAV file is supported...aborting." << std::endl;
	}
}

