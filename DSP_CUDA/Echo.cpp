#include "WAVFilter.h"
#include "WAVHandler.h"
#include <cstring>

void FilterEchoCPU(Audio_WAV& origin)
{
	WAV_HEADER origin_header = origin.get_header();
	unsigned long body_size = origin_header.Subchunk2Size;
	char* origin_archive = new char[body_size];
	char* origin_bytes = origin.get_audio();

	std::memcpy((char*)origin_archive, (char *)origin_bytes, body_size);

	int offset = origin_header.sampleRate / 4;
	for (int i = offset + 1; i < body_size; i++)
	{
		origin_bytes[i] = origin_archive[i] + origin_archive[i - offset] * 0.5;
	}
}