#pragma once

#include "Device.h"
#include "SynthDevice.h"
#include <cstdint>

namespace WaveSabreCore
{
	class Barley : public SynthDevice
	{
	public:
		constexpr static auto MaxGrains = 64;

		enum class ParamIndices
		{
			GrainPosition,
			GrainSize,

			GrainPositionSpread,
			GrainSizeSpread,

			BufferSize,
			CrossFade,

			Probability,
			Density,

			StereoSpread,

			NumParams
		};

		Barley();
		virtual ~Barley();

		virtual void Run(double songPosition, float **inputs, float **outputs, int numSamples);
		virtual void Render(double songPosition, float** inputs, float **outputs, int numSamples);

		virtual void SetParam(int index, float value);
		virtual float GetParam(int index) const;

	int prevFreeGrainCount;

	protected:
		float *buffer;
		int bufferSize;
		int xfadeSize;
		int writeHead;

		float bufferSizeParam;
		float xfadeAmt;

		float grainPosition, grainPositionSpread;
		float grainSize, grainSizeSpread;
		float stereoSpread;

		float probability;
		float density;

		int nextGrainCountdown;


		struct Grain
		{
			int firstSample;
			int size;
			int preDelay;
			double phase;
			double phaseIncrement;

			float envSmooth;
			float envSlope;
			float envPhase;
			float envPhaseIncrement;
			
			float gainL;
			float gainR;

			bool active;

			void Run(Barley* device, float** outputs, int numSamples);
		};

		Grain grains[MaxGrains];
		int freeGrains[MaxGrains];
		int freeGrainCount;

		void ScheduleGrain(Grain* grain, int preDelay);

		void ReadFrame(double samplePos, float* frame);

		class BarleyVoice : public Voice
		{
		public:
			BarleyVoice(Barley* device);
			
			virtual SynthDevice *GetSynthDevice() const;
			virtual void Run(double songPosition, float **outputs, int numSamples);

			Barley* device;
		};

		BarleyVoice* activeVoices[maxVoices];
		int activeVoiceCount;
	};
}

