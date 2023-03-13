#include <WaveSabreCore/Washington.h>
#include <WaveSabreCore/Helpers.h>

#include <math.h>

namespace WaveSabreCore
{
	Washington::Washington()
		: SynthDevice((int)ParamIndices::NumParams)
	{
		for (int i = 0; i < maxVoices; i++) voices[i] = new WashingtonVoice(this);

		attack = 1.0f;
		decay = 5.0f;
		sustain = .5f;
		release = 1.5f;

		keyMod = 0.0f;
		gain = 1.0f;
	}

	void Washington::SetParam(int index, float value)
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::Attack: attack = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Decay: decay = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::Sustain: sustain = value; break;
		case ParamIndices::Release: release = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::KeyMod: keyMod = value; break;
		case ParamIndices::Gain: gain = value; break;
		default: break;
		}
	}

	float Washington::GetParam(int index) const
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::Attack: return Helpers::EnvValueToScalar(attack);
		case ParamIndices::Decay: return Helpers::EnvValueToScalar(decay);
		case ParamIndices::Sustain: return sustain;
		case ParamIndices::Release: return Helpers::EnvValueToScalar(release);
		case ParamIndices::KeyMod: return keyMod;
		case ParamIndices::Gain: return gain;
		default: return 0.0f;
		}
	}

	Washington::WashingtonVoice::WashingtonVoice(Washington *washington)
		: washington(washington)
	{
	}

	SynthDevice *Washington::WashingtonVoice::GetSynthDevice() const
	{
		return washington;
	}

	void Washington::WashingtonVoice::Run(double songPosition, float **outputs, int numSamples)
	{
		constexpr auto invMaxNote = 1.0f / 127.0f;

		for (int i = 0; i < numSamples; i++)
		{
			float mod = 1.0f - washington->keyMod * (1.0f - GetNote() * invMaxNote);
			float value = env.GetValue() * mod * washington->gain;

			outputs[0][i] += value;
			outputs[1][i] += value;

			env.Next();
			if (env.State == EnvelopeState::Finished)
			{
				IsOn = false;
				break;
			}
		}
	}

	void Washington::WashingtonVoice::NoteOn(int note, int velocity, float detune, float pan)
	{
		Voice::NoteOn(note, velocity, detune, pan);

		env.Attack = washington->attack;
		env.Decay = washington->decay;
		env.Sustain = washington->sustain;
		env.Release = washington->release;
		env.Trigger();
	}

	void Washington::WashingtonVoice::NoteOff()
	{
		env.Off();
	}
}
