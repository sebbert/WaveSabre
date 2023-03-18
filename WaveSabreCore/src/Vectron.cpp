#include <WaveSabreCore/Vectron.h>
#include <WaveSabreCore/Helpers.h>

#include <math.h>

namespace WaveSabreCore
{
	Vectron::Vectron()
		: SynthDevice((int)ParamIndices::NumParams)
	{
		for (int i = 0; i < maxVoices; i++) voices[i] = new VectronVoice(this);

		osc1.X.Value = 0.5f;
		osc1.X.Env1Amt = 0.0f;
		osc1.X.Env2Amt = 0.0f;

		osc1.Y.Value = 0.0f;
		osc1.Y.Env1Amt = 0.0f;
		osc1.Y.Env2Amt = 0.0f;

		gain.Value = 0.0f;
		gain.Env1Amt = 0.8f; // Env1 controls gain
		gain.Env2Amt = 0.0f;

		env1.Attack = 1.0f;
		env1.Decay = 5.0f;
		env1.Sustain = 0.5f;
		env1.Release = 1.5f;

		env2.Attack = 1.0f;
		env2.Decay = 5.0f;
		env2.Sustain = 0.5f;
		env2.Release = 1.5f;
	}

	void Vectron::SetParam(int index, float value)
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::Osc1X: osc1.X.Value = value; break;
		case ParamIndices::Osc1XEnv1Amt: osc1.X.Env1Amt = Helpers::UnitToSigned(value); break;
		case ParamIndices::Osc1XEnv2Amt: osc1.X.Env2Amt = Helpers::UnitToSigned(value); break;

		case ParamIndices::Osc1Y: osc1.Y.Value = value; break;
		case ParamIndices::Osc1YEnv1Amt: osc1.Y.Env1Amt = Helpers::UnitToSigned(value); break;
		case ParamIndices::Osc1YEnv2Amt: osc1.Y.Env2Amt = Helpers::UnitToSigned(value); break;

		case ParamIndices::Gain: gain.Value = value; break;
		case ParamIndices::GainEnv1Amt: gain.Env1Amt = Helpers::UnitToSigned(value); break;
		case ParamIndices::GainEnv2Amt: gain.Env2Amt = Helpers::UnitToSigned(value); break;

		case ParamIndices::Env1Attack: env1.Attack = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Env1Decay: env1.Decay = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Env1Sustain: env1.Sustain = value; break;
		case ParamIndices::Env1Release: env1.Release = Helpers::ScalarToEnvValue(value); break;

		case ParamIndices::Env2Attack: env2.Attack = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Env2Decay: env2.Decay = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Env2Sustain: env2.Sustain = value; break;
		case ParamIndices::Env2Release: env2.Release = Helpers::ScalarToEnvValue(value); break;
		}
	}

	float Vectron::GetParam(int index) const
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::Osc1X: return osc1.X.Value;
		case ParamIndices::Osc1XEnv1Amt: return Helpers::SignedToUnit(osc1.X.Env1Amt);
		case ParamIndices::Osc1XEnv2Amt: return Helpers::SignedToUnit(osc1.X.Env2Amt);

		case ParamIndices::Osc1Y: return osc1.Y.Value;
		case ParamIndices::Osc1YEnv1Amt: return Helpers::SignedToUnit(osc1.Y.Env1Amt);
		case ParamIndices::Osc1YEnv2Amt: return Helpers::SignedToUnit(osc1.Y.Env2Amt);

		case ParamIndices::Gain: return gain.Value;
		case ParamIndices::GainEnv1Amt: return Helpers::SignedToUnit(gain.Env1Amt);
		case ParamIndices::GainEnv2Amt: return Helpers::SignedToUnit(gain.Env2Amt);

		case ParamIndices::Env1Attack: return Helpers::EnvValueToScalar(env1.Attack);
		case ParamIndices::Env1Decay: return Helpers::EnvValueToScalar(env1.Decay);
		case ParamIndices::Env1Sustain: return env1.Sustain;
		case ParamIndices::Env1Release: return Helpers::EnvValueToScalar(env1.Release);

		case ParamIndices::Env2Attack: return Helpers::EnvValueToScalar(env2.Attack);
		case ParamIndices::Env2Decay: return Helpers::EnvValueToScalar(env2.Decay);
		case ParamIndices::Env2Sustain: return env2.Sustain;
		case ParamIndices::Env2Release: return Helpers::EnvValueToScalar(env2.Release);
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

	inline float Vectron::ModParam::GetValue(const Vectron::Mod *mod) const
	{
		float sum = Value;
		sum += mod->Env1 * Env1Amt;
		sum += mod->Env2 * Env2Amt;
		return Helpers::Clamp(sum, 0.0f, 1.0f);
	}

	inline float Vectron::Oscillator::Next(double phaseIncrement, const Mod *mod)
	{
		double x = X.GetValue(mod);
		double y = Y.GetValue(mod) * 4.0;
	
		double distPhase;
		if (Phase <= x)
		{
			distPhase = (y * Phase) / x;
		}
		else 
		{
			distPhase = (1.0 - y) * (Phase - x) / (1.0 - x) + y;
		}

		float output = (float) -Helpers::FastCos(2 * M_PI * distPhase);

		Phase += phaseIncrement;
		if (Phase >= 1.0)
		{
			Phase -= 1.0;
		}

		return output;
	}

	void Vectron::VectronVoice::Run(double songPosition, float **outputs, int numSamples)
	{
		Mod mod;

		double invSampleRate = 1.0 / Helpers::CurrentSampleRate;
		
		for (auto i = 0; i < numSamples; ++i)
		{
			double note = GetNote();
			double freq = Helpers::NoteToFreq(note);
			double phaseIncrement = freq * invSampleRate;

			mod.Env1 = env1.GetValue();
			mod.Env2 = env2.GetValue();

			float osc1Output = osc1.Next(phaseIncrement, &mod);

			float mix = osc1Output * gain.GetValue(&mod);

			outputs[0][i] += mix * panLeft;
			outputs[1][i] += mix * panRight;

			env1.Next();
			env2.Next();

			if (env1.State == EnvelopeState::Finished &&
				env2.State == EnvelopeState::Finished)
			{
				IsOn = false;
				break;
			}
		}
	}

	void Vectron::VectronVoice::NoteOn(int note, int velocity, float detune, float pan)
	{
		Voice::NoteOn(note, velocity, detune, pan);

		panLeft = Helpers::PanToScalarLeft(Pan);
		panRight = Helpers::PanToScalarRight(Pan);

		osc1 = vectron->osc1;
		osc1.Phase = Helpers::SignedToUnit((double)Helpers::RandFloat());

		gain = vectron->gain;

		env1 = vectron->env1;
		env1.Trigger();

		env2 = vectron->env2;
		env2.Trigger();
	}

	void Vectron::VectronVoice::NoteOff()
	{
		env1.Off();
		env2.Off();
	}
}
