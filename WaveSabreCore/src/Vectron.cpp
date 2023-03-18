#include <WaveSabreCore/Vectron.h>
#include <WaveSabreCore/Helpers.h>

#include <math.h>

namespace WaveSabreCore
{
	constexpr static float yRange = 8;

	Vectron::Vectron()
		: SynthDevice((int)ParamIndices::NumParams)
	{
		for (int i = 0; i < maxVoices; i++) voices[i] = new VectronVoice(this);

		osc1X = 0.5f;
		osc1Y = 0.5f;

		osc1AmpAttack = 1.0f;
		osc1AmpDecay = 5.0f;
		osc1AmpSustain = 0.75f;
		osc1AmpRelease = 1.5f;

		osc1YAttack = 1.0f;
		osc1YDecay = 5.0f;
		osc1YSustain = 0.75f;
		osc1YRelease = 1.5f;

		osc1YEnvAmt = 0.0f;

		masterLevel = 0.8f;
	}

	void Vectron::SetParam(int index, float value)
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::Osc1X: osc1X = value; break;
		case ParamIndices::Osc1Y: osc1Y = value * (2 * yRange) - yRange; break;
		
		case ParamIndices::Osc1AmpAttack: osc1AmpAttack = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Osc1AmpDecay: osc1AmpDecay = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Osc1AmpSustain: osc1AmpSustain = value; break;
		case ParamIndices::Osc1AmpRelease: osc1AmpRelease = Helpers::ScalarToEnvValue(value); break;
		
		case ParamIndices::Osc1YAttack: osc1YAttack = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Osc1YDecay: osc1YDecay = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Osc1YSustain: osc1YSustain = value; break;
		case ParamIndices::Osc1YRelease: osc1YRelease = Helpers::ScalarToEnvValue(value); break;

		case ParamIndices::Osc1YEnvAmt: osc1YEnvAmt = (value - 0.5) * 2 * yRange; break;

		case ParamIndices::MasterLevel: masterLevel = value; break;
		}
	}

	float Vectron::GetParam(int index) const
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::Osc1X: return osc1X;
		case ParamIndices::Osc1Y: return (osc1Y + yRange) / (2 * yRange);

		case ParamIndices::Osc1AmpAttack: return Helpers::EnvValueToScalar(osc1AmpAttack);
		case ParamIndices::Osc1AmpDecay: return Helpers::EnvValueToScalar(osc1AmpDecay);
		case ParamIndices::Osc1AmpSustain: return osc1AmpSustain;
		case ParamIndices::Osc1AmpRelease: return Helpers::EnvValueToScalar(osc1AmpRelease);

		case ParamIndices::Osc1YAttack: return Helpers::EnvValueToScalar(osc1YAttack);
		case ParamIndices::Osc1YDecay: return Helpers::EnvValueToScalar(osc1YDecay);
		case ParamIndices::Osc1YSustain: return osc1YSustain;
		case ParamIndices::Osc1YRelease: return Helpers::EnvValueToScalar(osc1YRelease);

		case ParamIndices::Osc1YEnvAmt: return osc1YEnvAmt / (2 * yRange) + 0.5;

		case ParamIndices::MasterLevel: return masterLevel;

		default: return 0.0f;
		}
	}

	Vectron::VectronVoice::VectronVoice(Vectron *vectron)
	{
		this->vectron = vectron;
	}

	SynthDevice *Vectron::VectronVoice::GetSynthDevice() const
	{
		return vectron;
	}

	inline float Vectron::Oscillator::Next(double phaseIncrement)
	{
		float y = Y + YEnvAmt * YEnv.GetValue();

		float distPhase;

		if (Phase <= (double)X)
		{
			distPhase = (y * Phase) / X;
		}
		else 
		{
			distPhase = (1 - y) * ((Phase - X) / (1.0f - X)) + y;
		}

		float output = -Helpers::FastCos(2 * M_PI * distPhase);

		output *= AmpEnv.GetValue();

		Phase += phaseIncrement;
		if (Phase >= 1.0)
		{
			Phase -= 1.0;
		}

		AmpEnv.Next();
		YEnv.Next();

		return output;
	}

	void Vectron::VectronVoice::Run(double songPosition, float **outputs, int numSamples)
	{
		float masterLevel = vectron->masterLevel;

		double freq = Helpers::NoteToFreq(Note);
		double phaseIncrement = freq / Helpers::CurrentSampleRate;

		float gainLeft = masterLevel * Helpers::PanToScalarLeft(Pan);
		float gainRight = masterLevel * Helpers::PanToScalarRight(Pan);

		for (auto i = 0; i < numSamples; ++i)
		{
			auto osc1Output = osc1.Next(phaseIncrement);

			outputs[0][i] += osc1Output * gainLeft;
			outputs[1][i] += osc1Output * gainRight;
		}

		if (osc1.AmpEnv.State == EnvelopeState::Finished)
		{
			IsOn = false;
		}
	}

	void Vectron::VectronVoice::NoteOn(int note, int velocity, float detune, float pan)
	{
		Voice::NoteOn(note, velocity, detune, pan);

		osc1.Phase = (double)Helpers::RandFloat() * 0.5 + 0.5;

		osc1.X = vectron->osc1X;
		osc1.Y = vectron->osc1Y;

		osc1.YEnvAmt = vectron->osc1YEnvAmt;

		osc1.AmpEnv.Attack = vectron->osc1AmpAttack;
		osc1.AmpEnv.Decay = vectron->osc1AmpDecay;
		osc1.AmpEnv.Sustain = vectron->osc1AmpSustain;
		osc1.AmpEnv.Release = vectron->osc1AmpRelease;
		osc1.AmpEnv.Trigger();

		osc1.YEnv.Attack = vectron->osc1YAttack;
		osc1.YEnv.Decay = vectron->osc1YDecay;
		osc1.YEnv.Sustain = vectron->osc1YSustain;
		osc1.YEnv.Release = vectron->osc1YRelease;
		osc1.YEnv.Trigger();
	}

	void Vectron::VectronVoice::NoteOff()
	{
		osc1.AmpEnv.Off();
		osc1.YEnv.Off();
	}
}
