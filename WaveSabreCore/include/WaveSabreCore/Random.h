#pragma once

namespace WaveSabreCore
{
	class Random
	{
	public:
		Random()
		{
			Reset();
		}

		void Reset()
		{
			Seed = 1;
		}

		float Next();

		int Seed;
	};
}