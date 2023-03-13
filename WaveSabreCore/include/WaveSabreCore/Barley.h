#pragma once

#include "Device.h"
#include <cstdint>

namespace WaveSabreCore
{
	class Barley : public SynthDevice
	{
	public:
		constexpr static auto MaxGrains = 128;
		constexpr static auto MaxBlockSize = 256;

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

	protected:
		float *buffer;
		int bufferSize;
		int xfadeSize;
		size_t writeHead;

		float bufferSizeParam;
		float xfadeAmt;

		float grainPosition, grainPositionSpread;
		float grainSize, grainSizeSpread;
		float stereoSpread;

		float probability;
		float density;




		struct Grain
		{
			double position;
			double positionIncrement;
			int size;
			int preDelay;

			float envSmooth;
			float envSlope;
			
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
			virtual void NoteOn(int note, int velocity, float detune, float pan);
			virtual void NoteOff();
			Barley* device;
		};

		BarleyVoice* activeVoices[maxVoices];
		int activeVoiceCount;
	};
}

