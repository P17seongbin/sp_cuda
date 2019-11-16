#include <string>
#include <iostream>
#include <fstream>
#include <map>


#include "Filter.cuh"
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
	
	//after opening file, determine given file's type 
	//일단은 wav만 한다고 가정
	std::string expender = find_expender(filename);
	if (expender == "wav")
	{
		//Create WAV object
		Audio_WAV item(audiofile,filename);
		//Processing WAV object(With CUDA)
		AudioHandler_WAV(item, argv, argc, true);
		//print WAV object
		Create_WAVfile(item);
	}
	else
	{
		std::cout << "only WAV file is supported...aborting." << std::endl;
	}
}

