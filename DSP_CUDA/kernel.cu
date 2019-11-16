#include "CUDAlib.cuh"
#include "Filter.cuh"
#include "FileHandler.h"

#include <string>
#include <iostream>
#include <map>

/*
argument list:
-echo x y : apply echo effect x times with delay of y seconds
*/
int main(int argc, char* argv[])
{
	static std::map<std::string, FileOpenerType> opener_map;
	if (!opener_created)
	{
		opener_map.insert(make_pair("wav", open_wav));
		opener_created = true;
	}
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

	//Open file
	if ((audiofile = fopen(filename.c_str(), "r")) == NULL)
	{	
		std::cout << "Something went wrong while opening file \"" << filename << "\"" << std::endl;
		std::cout << "Aborting...." << std::endl;
	}
	
	//after opening file, determine given file's type 
	//do something 
	//일단은 wav만 한다고 가정


}