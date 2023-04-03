#pragma once

namespace WaveSabreCore
{
	class Random
	{
	public:
		inline void Reset()
		{
			Seed = 1;
		}

		float Next();

		int Seed;
	};
}