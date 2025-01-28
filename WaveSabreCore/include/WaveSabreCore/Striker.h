#ifndef __WAVESABRECORE_STRIKER_H__
#define __WAVESABRECORE_STRIKER_H__

#include "SynthDevice.h"
#include "Envelope.h"
#include "Random.h"

namespace WaveSabreCore
{
	class Striker : public SynthDevice
	{
	public:
		enum class ParamIndices
		{
			NoiseImpactLevel,
			SineImpactLevel,


			NumParams,
		};

		Striker();

		virtual void SetParam(int index, float value);
		virtual float GetParam(int index) const;

	protected:
		float noiseImpactLevel, sineImpactLevel;

		class VariableDelay
		{
		public:
			VariableDelay(float capacityMs);
			~VariableDelay();

			void SetLengthMs(float lengthMs);
			void SetLengthSamples(int length);

			void Write(float sample);
			float Read();

			float ProcessAllpass(float input, float gain);

		private:
			int advanceHead(int head) const;

			float *buffer;
			int capacity;
			int length;
			int readHead;
			int writeHead;
		};

		class StrikerVoice : public Voice
		{
		public:
			StrikerVoice(Striker *striker);

			virtual WaveSabreCore::SynthDevice *GetSynthDevice() const;

			virtual void Run(double songPosition, float **outputs, int numSamples);

			virtual void NoteOn(int note, int velocity, float detune, float pan);
			virtual void NoteOff();

		private:
			Striker *striker;
			int waveLengthSamples;
			int currentSamples;
			VariableDelay comb, allpass;
			Random noise;
			Envelope pitchEnv;

		};
	};
}

#endif
