#pragma once
#include "AudioEntry.h"

Audio_WAV AudioHandler_WAV(Audio_WAV input, char* argv[], int argc, bool CUDAMode);

void Create_WAVfile(Audio_WAV result);