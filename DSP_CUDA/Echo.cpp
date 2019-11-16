#include "WAVFilter.cuh"
#include <cstring>

Audio_WAV FilterEchoCPU(Audio_WAV origin)
{
	std::cout << "T";

	WAV_HEADER origin_header = origin.get_header();
	unsigned long body_size = origin_header.Subchunk2Size;
	unsigned char* origin_archive = (unsigned char*)calloc(body_size, 1);
	unsigned char* origin_bytes = origin.get_audio();
	strncpy((char*)origin_archive, (char *)origin_bytes, body_size);
	int offset = origin_header.sampleRate / 8;
	for (int i = offset; i < body_size; i++)
	{
		origin_bytes[i] += 0.5 * origin_archive[i - offset];
	}
	return origin;
}