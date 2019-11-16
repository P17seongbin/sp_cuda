#pragma once
#define READ_CHUNK 1000
#include <iostream>
#include <fstream>
enum audio_type { WAV, MP3, FLAC };

struct  WAV_HEADER {
	char                RIFF[4];        // RIFF Header      Magic header
	unsigned long       ChunkSize;      // RIFF Chunk Size  
	char                WAVE[4];        // WAVE Header      
	char                fmt[4];         // FMT header       
	unsigned long       Subchunk1Size;  // Size of the fmt chunk                                
	unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
	unsigned short      NumOfChannel;   // Number of channels 1=Mono 2=Sterio                   
	unsigned long       sampleRate;     // Sampling Frequency in Hz                             
	unsigned long       bitRate;        // bytes per second 
	unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo 
	unsigned short      bitsPerSample;  // Number of bits per sample   

	char                Subchunk2ID[4]; // "data"  string   
	unsigned long       Subchunk2Size;  // Sampled data length    

};

class Audio_WAV {
private:

	WAV_HEADER header;
	//byte list, memory should allocated with calloc
	 char* audio;
public:
	std::string filename;
	WAV_HEADER get_header()
	{
		return header;
	}
	  char* get_audio()
	{
		return audio;
	}
	Audio_WAV(std::ifstream& file, std::string name)
	{
		filename = name;

		file.read((char*)(&header), sizeof(WAV_HEADER));
		audio = new  char[header.Subchunk2Size];
		long size = header.Subchunk2Size;


		int i = 0;
		for (i = 0; i < size - READ_CHUNK; i += READ_CHUNK)
		{
			file.read((char *)&audio[i], READ_CHUNK);
		}
		file.read((char*)&audio[i], size % READ_CHUNK);

	}

	~Audio_WAV()
	{
		//free(audio);
	}

};