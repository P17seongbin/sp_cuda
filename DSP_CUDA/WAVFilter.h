#include "CUDALib.cuh"
#include "AudioEntry.h"

void FilterEcho(Audio_WAV& origin, bool useCUDA = false, double delay = 0.25, double gain = 0.5);