#include "WaveSabreCore/Random.h"

namespace WaveSabreCore
{
	float Random::Next()
	{
		return (float)((Seed *= 0x15a4e35) % 255) / 255.0f;
	}
}