#include "CUDALib.cuh"
#include "AudioEntry.h"

void FilterEchoCUDA(Audio_WAV& origin);
void FilterEchoCPU(Audio_WAV& origin);