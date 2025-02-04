#ifndef __WAVESABRECORE_STRIKER_H__
#define __WAVESABRECORE_STRIKER_H__

#include "SynthDevice.h"
#include "Envelope.h"
#include "Random.h"
#include "BiquadFilter.h"

namespace WaveSabreCore
{
	class Striker : public SynthDevice
	{
	public:
		enum class ParamIndices
		{
			NoiseImpulseLevel,
			SineImpulseLevel,

			Feedback,
			Damping,
			
			AllpassGain,
			AllpassFreq,
			AllpassRatioCoarse,
			AllpassRatioFine,

			PitchAttack,
			PitchDecay,
			PitchSustain,
			PitchRelease,

			AmpAttack,
			AmpDecay,
			AmpSustain,
			AmpRelease,

			NumParams,
		};

		Striker();

		virtual void SetParam(int index, float value);
		virtual float GetParam(int index) const;

	protected:
		static constexpr float maxAllpassRatioCoarse = 8.0;

		float noiseImpulseLevel;
		float sineImpulseLevel;
		float feedback;
		float allpassGain, allpassFreq, allpassRatioCoarse, allpassRatioFine;
		float damping;
		float pitchAttack, pitchDecay, pitchSustain, pitchRelease;
		float ampAttack, ampDecay, ampSustain, ampRelease;

		class VariableDelay
		{
		public:
			VariableDelay(float capacityMs);
			~VariableDelay();

			void Clear();

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

		class OnePoleFilter
		{
		public:
			float ProcessLowpass(float input);
			float ProcessHighpass(float input);

			void Reset();

			void SetFreq(double freq);
			void SetCoef(float newCoef);

			static float FreqToCoef(double freq);

		private:
			float coef;
			float state;
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
			double noteFreq;
			double allpassFreq;
			float velocity;
			int waveLengthSamples;
			int elapsedSamples;
			VariableDelay combDelay, allpassDelay;
			OnePoleFilter combHighpassFilter, dampFilter;
			Random noise;
			Envelope pitchEnv, ampEnv;
		};
	};
}

#endif
