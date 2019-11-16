#include "CUDALib.cuh"
#include "AudioEntry.h"

Audio_WAV FilterEchoCUDA(Audio_WAV origin);
void FilterEchoCPU(Audio_WAV& origin);