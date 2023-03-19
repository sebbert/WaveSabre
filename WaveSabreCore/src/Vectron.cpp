#include <WaveSabreCore/Vectron.h>
#include <WaveSabreCore/Helpers.h>

#include <math.h>

#define SET_MOD_PARAM(INDEX, PARAM) \
	case INDEX:          PARAM.Value = value; break; \
	case INDEX##Env1Amt: PARAM.Env1Amt = Helpers::UnitToSigned(value); break; \
	case INDEX##Env2Amt: PARAM.Env2Amt = Helpers::UnitToSigned(value); break;

#define GET_MOD_PARAM(INDEX, PARAM) \
	case INDEX:          return PARAM.Value; \
	case INDEX##Env1Amt: return Helpers::SignedToUnit(PARAM.Env1Amt); \
	case INDEX##Env2Amt: return Helpers::SignedToUnit(PARAM.Env2Amt);


#define SET_ADSR(INDEX, ENV) \
	case INDEX##Attack:  ENV.Attack  = Helpers::ScalarToEnvValue(value); break; \
	case INDEX##Decay:   ENV.Decay   = Helpers::ScalarToEnvValue(value); break; \
	case INDEX##Sustain: ENV.Sustain = value; break; \
	case INDEX##Release: ENV.Release = Helpers::ScalarToEnvValue(value); break;

#define GET_ADSR(INDEX, ENV) \
	case INDEX##Attack:  return Helpers::EnvValueToScalar(ENV.Attack); \
	case INDEX##Decay:   return Helpers::EnvValueToScalar(ENV.Decay); \
	case INDEX##Sustain: return ENV.Sustain; \
	case INDEX##Release: return Helpers::EnvValueToScalar(ENV.Release);

static double IncPhase(double phase, double increment)
{
	phase += increment;
	if (phase >= 1.0)
	{
		phase -= 1.0;
	}
	return phase;
}

namespace WaveSabreCore
{
	Vectron::Vectron()
		: SynthDevice((int)ParamIndices::NumParams)
	{
		for (int i = 0; i < maxVoices; i++) voices[i] = new VectronVoice(this);

		phaseMod.Scale.Value = 0.5f;
		phaseMod.Scale.Env1Amt = 0.0f;
		phaseMod.Scale.Env2Amt = 0.0f;

		phaseMod.Offset.Value = 0.0f;
		phaseMod.Offset.Env1Amt = 0.0f;
		phaseMod.Offset.Env2Amt = 0.0f;


		phaseMod.X.Scale.Value = 0.5f;
		phaseMod.X.Scale.Env1Amt = 0.0f;
		phaseMod.X.Scale.Env2Amt = 0.0f;

		phaseMod.X.Offset.Value = 0.0f;
		phaseMod.X.Offset.Env1Amt = 0.0f;
		phaseMod.X.Offset.Env2Amt = 0.0f;

		phaseMod.X.Detune.Value = 0.5f;
		phaseMod.X.Detune.Env1Amt = 0.0f;
		phaseMod.X.Detune.Env2Amt = 0.0f;


		phaseMod.Y.Scale.Value = 0.5f;
		phaseMod.Y.Scale.Env1Amt = 0.0f;
		phaseMod.Y.Scale.Env2Amt = 0.0f;

		phaseMod.Y.Offset.Value = 0.0f;
		phaseMod.Y.Offset.Env1Amt = 0.0f;
		phaseMod.Y.Offset.Env2Amt = 0.0f;

		phaseMod.Y.Detune.Value = 0.5f;
		phaseMod.Y.Detune.Env1Amt = 0.0f;
		phaseMod.Y.Detune.Env2Amt = 0.0f;


		osc1.Offset.Value = 0.0f;
		osc1.Offset.Env1Amt = 0.0f;
		osc1.Offset.Env2Amt = 0.0f;

		osc1.Mod.Value = 0.3f;
		osc1.Mod.Env1Amt = 0.0f;
		osc1.Mod.Env2Amt = 0.0f;


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
		SET_MOD_PARAM(ParamIndices::ModScale, phaseMod.Scale)
		SET_MOD_PARAM(ParamIndices::ModOffset, phaseMod.Offset)

		SET_MOD_PARAM(ParamIndices::ModXScale, phaseMod.X.Scale)
		SET_MOD_PARAM(ParamIndices::ModXOffset, phaseMod.X.Offset)
		SET_MOD_PARAM(ParamIndices::ModXDetune, phaseMod.X.Detune)

		SET_MOD_PARAM(ParamIndices::ModYScale, phaseMod.Y.Scale)
		SET_MOD_PARAM(ParamIndices::ModYOffset, phaseMod.Y.Offset)
		SET_MOD_PARAM(ParamIndices::ModYDetune, phaseMod.Y.Detune)

		SET_MOD_PARAM(ParamIndices::Osc1Offset, osc1.Offset)
		SET_MOD_PARAM(ParamIndices::Osc1Mod, osc1.Mod)

		SET_ADSR(ParamIndices::Env1, env1)
		SET_ADSR(ParamIndices::Env2, env2)
		}
	}

	float Vectron::GetParam(int index) const
	{
		switch ((ParamIndices)index)
		{
		GET_MOD_PARAM(ParamIndices::ModScale, phaseMod.Scale)
		GET_MOD_PARAM(ParamIndices::ModOffset, phaseMod.Offset)

		GET_MOD_PARAM(ParamIndices::ModXScale, phaseMod.X.Scale)
		GET_MOD_PARAM(ParamIndices::ModXOffset, phaseMod.X.Offset)
		GET_MOD_PARAM(ParamIndices::ModXDetune, phaseMod.X.Detune)

		GET_MOD_PARAM(ParamIndices::ModYScale, phaseMod.Y.Scale)
		GET_MOD_PARAM(ParamIndices::ModYOffset, phaseMod.Y.Offset)
		GET_MOD_PARAM(ParamIndices::ModYDetune, phaseMod.Y.Detune)

		GET_MOD_PARAM(ParamIndices::Osc1Offset, osc1.Offset)
		GET_MOD_PARAM(ParamIndices::Osc1Mod, osc1.Mod)

		GET_ADSR(ParamIndices::Env1, env1)
		GET_ADSR(ParamIndices::Env2, env2)
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

	inline float Vectron::ModParam::GetValue(const Vectron::Modulation *mod) const
	{
		float sum = Value;
		sum += mod->Env1 * Env1Amt;
		sum += mod->Env2 * Env2Amt;
		return Helpers::Clamp(sum, 0.0f, 1.0f);
	}

	inline float Vectron::Oscillator::Next(double *phaseVar, double note, double modX, double modY, const Modulation *mod)
	{
		double freq = 0.5 * Helpers::NoteToFreq(note);
		double phaseIncrement = freq / Helpers::CurrentSampleRate;

		double phaseModAmt = Mod.GetValue(mod);
		double x = phaseModAmt * modX;
		double y = phaseModAmt * modY * 8.0;

		double phase = IncPhase(*phaseVar, Offset.GetValue(mod));
	
		double phaseDist;
		if (phase <= x)
		{
			phaseDist = (y * phase) / x;
		}
		else 
		{
			phaseDist = (1.0 - y) * (phase - x) / (1.0 - x) + y;
		}

		float output = (float)-Helpers::FastCos(2 * M_PI * phaseDist);

		*phaseVar = IncPhase(*phaseVar, phaseIncrement);

		return output;
	}

	inline double Vectron::ModOscillator::Next(double *phaseVar, double note, const Modulation *mod)
	{
		double detune = Helpers::UnitToSigned(Detune.GetValue(mod));
		double freq = 0.5 * Helpers::NoteToFreq(note + detune);
		double phaseIncrement = freq / Helpers::CurrentSampleRate;

		double phase = *phaseVar + Offset.GetValue(mod);
		double scale = Scale.GetValue(mod);

		double output = scale * Helpers::SignedToUnit(Helpers::FastCos(2.0 * M_PI * phase));

		*phaseVar = IncPhase(*phaseVar, phaseIncrement);

		return output;
	}

	void Vectron::VectronVoice::Run(double songPosition, float **outputs, int numSamples)
	{
		Modulation mod;

		double invSampleRate = 1.0 / Helpers::CurrentSampleRate;

		for (auto i = 0; i < numSamples; ++i)
		{
			mod.Env1 = env1.GetValue();
			mod.Env2 = env2.GetValue();

			double note = GetNote();

			double modScale = vectron->phaseMod.Scale.GetValue(&mod);
			double modOffset = vectron->phaseMod.Offset.GetValue(&mod);

			double modX = vectron->phaseMod.X.Next(&xModPhase, note, &mod);
			double modY = vectron->phaseMod.Y.Next(&yModPhase, note, &mod);
			float osc1Output = vectron->osc1.Next(&osc1Phase, note, modX, modY, &mod);

			float mix = osc1Output * mod.Env1;

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

		xModPhase = Helpers::SignedToUnit((double)Helpers::RandFloat());
		yModPhase = Helpers::SignedToUnit((double)Helpers::RandFloat());
		osc1Phase = Helpers::SignedToUnit((double)Helpers::RandFloat());

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
