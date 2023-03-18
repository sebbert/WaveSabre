#ifndef __WAVESABRECORE_VECTRON_H__
#define __WAVESABRECORE_VECTRON_H__

#include "SynthDevice.h"
#include "Envelope.h"

namespace WaveSabreCore
{
	class Vectron : public SynthDevice
	{
	public:
		enum class ParamIndices
		{
			Osc1X, Osc1XEnv1Amt, Osc1XEnv2Amt,
			Osc1Y, Osc1YEnv1Amt, Osc1YEnv2Amt,

			Gain, GainEnv1Amt, GainEnv2Amt,

			Env1Attack, Env1Decay, Env1Sustain, Env1Release,
			Env2Attack, Env2Decay, Env2Sustain, Env2Release,
			
			NumParams,
		};

		Vectron();

		virtual void SetParam(int index, float value);
		virtual float GetParam(int index) const;

		struct Mod
		{
			float Env1, Env2;
		};

		class ModParam
		{
		public:
			enum class ParamIndices
			{
				Value,
				Env1Amt, Env2Amt,

				NumParams,
			};

			inline float GetValue(const Mod* mod) const;

			float Value;
			float Env1Amt;
			float Env2Amt;
		};

		class Oscillator
		{
		public:
			inline float Next(double phaseIncrement, const Mod *mod);

			double Phase;

			ModParam X, Y;
		};

		class VectronVoice : public Voice
		{
		public:
			VectronVoice(Vectron *vectron);
			virtual WaveSabreCore::SynthDevice *GetSynthDevice() const;

			virtual void Run(double songPosition, float **outputs, int numSamples);

			virtual void NoteOn(int note, int velocity, float detune, float pan);
			virtual void NoteOff();

		private:
			Vectron *vectron;

			float panLeft, panRight;

			Oscillator osc1;
			ModParam gain;

			Envelope env1, env2;
		};

protected:
		Oscillator osc1;
		ModParam gain;

		Envelope env1, env2;
	};
}

#endif
