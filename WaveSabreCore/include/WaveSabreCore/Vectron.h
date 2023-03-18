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
			Osc1X,
			Osc1Y,

			Osc1AmpAttack,
			Osc1AmpDecay,
			Osc1AmpSustain,
			Osc1AmpRelease,

			Osc1YAttack,
			Osc1YDecay,
			Osc1YSustain,
			Osc1YRelease,

			Osc1YEnvAmt,

			MasterLevel,

			NumParams,
		};

		Vectron();

		virtual void SetParam(int index, float value);
		virtual float GetParam(int index) const;

	protected:

		class Oscillator
		{
		public:
			inline float Next(double phaseIncrement);

			double Phase;
			Envelope AmpEnv, YEnv;
			float X, Y;

			float YEnvAmt;
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

			Oscillator osc1;
		};

		float osc1X, osc1Y;
		float osc1AmpAttack, osc1AmpDecay, osc1AmpSustain, osc1AmpRelease;
		float osc1YAttack, osc1YDecay, osc1YSustain, osc1YRelease;
		float osc1YEnvAmt;

		float masterLevel;
	};
}

#endif
