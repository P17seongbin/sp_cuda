#pragma once
#include <iostream>

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

	FILE* origin_file;
	WAV_HEADER header;
	//byte list, memory should allocated with calloc
	unsigned char* audio;
public:
	std::string filename;
	WAV_HEADER get_header()
	{
		return header;
	}
	unsigned char* get_audio()
	{
		return audio;
	}
	Audio_WAV(FILE* file, std::string name)
	{
		filename = name;
		std::cout << name << std::endl;
		origin_file = file;
		fread(&header, sizeof(WAV_HEADER), 1, file);
		audio = (unsigned char*)calloc(header.Subchunk2Size, 1);
		fread(&audio, header.Subchunk2Size, 1, file);
	}

	~Audio_WAV()
	{
		free(audio);
	}

};