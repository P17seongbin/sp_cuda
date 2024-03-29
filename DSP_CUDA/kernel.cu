#include <string>
#include <iostream>
#include <map>
#include <sstream>

#include "Filter.cuh"
#include "AudioEntry.h"


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
		std::cout << "Type audio file path (WAV) : ";
		std::cin >> filename;	
	}
	else filename = argv[1];
	
	//Open file
	if ((audiofile = fopen(filename.c_str(), "r")) == NULL)
	{	
		std::cout << "Something went wrong while opening file \"" << filename << "\"" << std::endl;
		std::cout << "Aborting...." << std::endl;
	}
	
	//after opening file, determine given file's type 
	//do something 
	//일단은 wav만 한다고 가정
	std::string token;
	std::stringstream ss(filename);

	while (std::getline(ss, token, '.'));
	if (token == "wav")
	{

	}
	else
	{
		std::cout << "only WAV file is supported...Aborting." << std::endl;
		fclose(audiofile);
	}
}

