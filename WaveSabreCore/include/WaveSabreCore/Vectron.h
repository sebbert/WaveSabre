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
			ModScale,   ModScaleEnv1Amt,   ModScaleEnv2Amt,
			ModOffset,  ModOffsetEnv1Amt,  ModOffsetEnv2Amt,

			ModXScale,  ModXScaleEnv1Amt,  ModXScaleEnv2Amt,
			ModXOffset, ModXOffsetEnv1Amt, ModXOffsetEnv2Amt,
			ModXDetune, ModXDetuneEnv1Amt, ModXDetuneEnv2Amt,

			ModYScale,  ModYScaleEnv1Amt,  ModYScaleEnv2Amt,
			ModYOffset, ModYOffsetEnv1Amt, ModYOffsetEnv2Amt,
			ModYDetune, ModYDetuneEnv1Amt, ModYDetuneEnv2Amt,

			Osc1Offset, Osc1OffsetEnv1Amt, Osc1OffsetEnv2Amt,
			Osc1Mod,    Osc1ModEnv1Amt,    Osc1ModEnv2Amt,

			Env1Attack, Env1Decay, Env1Sustain, Env1Release,
			Env2Attack, Env2Decay, Env2Sustain, Env2Release,

			NumParams,
		};

		Vectron();

		virtual void SetParam(int index, float value);
		virtual float GetParam(int index) const;

		struct Modulation
		{
			float Env1, Env2;
		};

		class ModParam
		{
		public:
			inline float GetValue(const Modulation *mod) const;

			float Value;
			float Env1Amt;
			float Env2Amt;
		};

		class ModOscillator
		{
		public:
			double Next(double *phaseVar, double note, const Modulation *mod);

			ModParam Scale, Offset, Detune;

			double Phase;
		};

		class PhaseModulator
		{
		public:
			ModParam Scale, Offset;

			ModOscillator X, Y;
		};

		class Oscillator
		{
		public:
			inline float Next(double *phaseVar, double note, double modX, double modY, const Modulation *mod);

			ModParam Offset;
			ModParam Mod;
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

			double xModPhase, yModPhase, osc1Phase;
			Envelope env1, env2;
		};

protected:
		PhaseModulator phaseMod;
		Oscillator osc1;
		Envelope env1, env2;
	};
}

#endif
