#ifndef __WAVESABRECORE_HARVESTER_H__
#define __WAVESABRECORE_HARVESTER_H__

#include "Device.h"
#include <cstdint>

namespace clouds { class GranularProcessor; }

namespace WaveSabreCore
{
	class Harvester : public Device
	{
	public:
		enum class ParamIndices
		{
			Position,
			Size,
			StereoSpread,
			Reverb,
			DryWet,

			NumParams
		};

		Harvester();
		virtual ~Harvester();

		virtual void Run(double songPosition, float **inputs, float **outputs, int numSamples);

		virtual void SetParam(int index, float value);
		virtual float GetParam(int index) const;

	private:
		clouds::GranularProcessor* processor;

		uint8_t buffer_large[118784];
		uint8_t buffer_small[65536];
	};
}


#endif
