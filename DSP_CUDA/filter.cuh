#include "cuda_lib.cuh"
#include <string>

enum audio_type {WAV, MP3, FLAC};

bool echo(std::string wav_input_path, std::string wav_output_path);