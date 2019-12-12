#include "CUDALib.cuh"
#include "AudioEntry.h"

void FilterEcho(Audio_WAV& origin, bool useCUDA = false, double delay = 0.5, double gain = 0.8);
void FilterAllpass(Audio_WAV& origin, bool useCUDA, double delay, double gain, double RT);
void FilterCompressor(Audio_WAV& origin, bool useCUDA, double threshold, double ratio);