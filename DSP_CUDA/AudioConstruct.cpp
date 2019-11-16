#include <sstream>
#include <iostream>
#include <map>
//if success to open given file, return True.
//else, return false
typedef FILE* (*FileOpenerType)(std::string); 

//wav file opener, 임시로 분리했지만 크게 의미는 없는듯
FILE* open_wav(std::string file_path)
{
	std::cout << file_path.c_str() << std::endl;
	return fopen(file_path.c_str(), "r");
}

FILE* open_audio(std::string file_path, audio_type* type)
{
	std::string original_filepath = file_path;
	static bool opener_created = false;
	static std::map<std::string, FileOpenerType> opener_map;
	if (!opener_created)
	{
		opener_map.insert(make_pair("wav", open_wav));
		opener_created = true;
	}
	std::string token;
	std::stringstream ss(file_path);

	while (std::getline(ss, token, '.')) {

	}
	if (opener_map.count(token) != 1)
	{
		return NULL;
	}
	else
		return opener_map[token](original_filepath);

}

