#include <WaveSabreCore/Striker.h>
#include <WaveSabreCore/Helpers.h>

#include <math.h>

namespace WaveSabreCore
{
	Striker::Striker()
		: SynthDevice((int)ParamIndices::NumParams)
	{
		for (int i = 0; i < maxVoices; i++) voices[i] = new StrikerVoice(this);

		noiseImpactLevel = 0.5;
		sineImpactLevel = 0.5;

		ampAttack = 1.0;
		ampDecay = Helpers::ScalarToEnvValue(0.5);
		ampSustain = 1.0;
		ampRelease = 100.0;

		pitchAttack = 1.0;
		pitchDecay = 400.0;
		pitchSustain = 0.0;
		pitchRelease = 1.0;
	}

	void Striker::SetParam(int index, float value)
	{
		switch ((ParamIndices)index)
		{
			case ParamIndices::NoiseImpactLevel: noiseImpactLevel = value; break;
			case ParamIndices::SineImpactLevel: sineImpactLevel = value; break;

			case ParamIndices::AmpAttack: ampAttack = Helpers::ScalarToEnvValue(value); break;
			case ParamIndices::AmpDecay: ampDecay = Helpers::ScalarToEnvValue(value); break;
			case ParamIndices::AmpSustain: ampSustain = value; break;
			case ParamIndices::AmpRelease: ampRelease = Helpers::ScalarToEnvValue(value); break;

			case ParamIndices::PitchAttack: pitchAttack = Helpers::ScalarToEnvValue(value); break;
			case ParamIndices::PitchDecay: pitchDecay = Helpers::ScalarToEnvValue(value); break;
			case ParamIndices::PitchSustain: pitchSustain = value; break;
			case ParamIndices::PitchRelease: pitchRelease = Helpers::ScalarToEnvValue(value); break;

		}
	}

	float Striker::GetParam(int index) const
	{
		switch ((ParamIndices)index)
		{
			case ParamIndices::NoiseImpactLevel: return noiseImpactLevel;
			case ParamIndices::SineImpactLevel: return sineImpactLevel;

			case ParamIndices::AmpAttack: return Helpers::EnvValueToScalar(ampAttack);
			case ParamIndices::AmpDecay: return Helpers::EnvValueToScalar(ampDecay);
			case ParamIndices::AmpSustain: return ampSustain;
			case ParamIndices::AmpRelease: return Helpers::EnvValueToScalar(ampRelease);
			
			case ParamIndices::PitchAttack: return Helpers::EnvValueToScalar(pitchAttack);
			case ParamIndices::PitchDecay: return Helpers::EnvValueToScalar(pitchDecay);
			case ParamIndices::PitchSustain: return pitchSustain;
			case ParamIndices::PitchRelease: return Helpers::EnvValueToScalar(pitchRelease);

			default: return 0.0;
		}
	}

	Striker::StrikerVoice::StrikerVoice(Striker *striker)
		: striker(striker)
		, comb(1000.0)
		, allpass(1000.0)
		, noise()
	{
	}

	SynthDevice *Striker::StrikerVoice::GetSynthDevice() const
	{
		return striker;
	}

	void Striker::StrikerVoice::Run(double songPosition, float **outputs, int numSamples)
	{
		for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
		{
			float impulse = 0.0;
			if (currentSamples <= waveLengthSamples)
			{
				if (striker->sineImpactLevel > 0.0f)
				{
					double t = (double)currentSamples / (double)waveLengthSamples;
					impulse += striker->sineImpactLevel * (float)Helpers::FastSin(t * 2.0 * M_PI);
				}
				if (striker -> noiseImpactLevel > 0.0f)
				{
					impulse += striker->noiseImpactLevel * noise.Next();
				}
			}

			double note = GetNote();
			double baseFreq = Helpers::NoteToFreq(note);

			pitchEnv.Next();
			float pitchEnvValue = pitchEnv.GetValue();
			float pitchEnvPow10 = powf(pitchEnvValue, 10.0f);
			float pitchEnvPow20 = powf(pitchEnvValue, 20.0f);
			double combFreq = baseFreq * (1.0 + (double)(pitchEnvPow10 * (velocity + 2.0f)));

			comb.SetLengthSamples((int)Helpers::FreqToWaveLengthSamples(combFreq));
			float combOut = comb.Read();
			float combIn = impulse + combOut * 0.98f;
			comb.Write(combIn);

			ampEnv.Next();
			float masterOut = combOut * ampEnv.GetValue();

			outputs[0][sampleIndex] += masterOut;
			outputs[1][sampleIndex] += masterOut;

			currentSamples += 1;
		}

		if (ampEnv.State == EnvelopeState::Finished)
		{
			IsOn = false;
		}
	}

	void Striker::StrikerVoice::NoteOn(int note, int velocityInt, float detune, float pan)
	{
		Voice::NoteOn(note, velocityInt, detune, pan);
		
		velocity = (float)velocityInt / 127.0;
		
		double baseFreq = Helpers::NoteToFreq(GetNote());
		waveLengthSamples = (int)(Helpers::CurrentSampleRate / baseFreq);
		currentSamples = 0;
		
		noise.Reset();

		pitchEnv.Attack = striker->pitchAttack;
		pitchEnv.Decay = striker->pitchDecay;
		pitchEnv.Sustain = striker->pitchSustain;
		pitchEnv.Release = striker->pitchRelease;
		pitchEnv.Trigger();

		ampEnv.Attack = striker->ampAttack;
		ampEnv.Decay = striker->ampDecay;
		ampEnv.Sustain = striker->ampSustain;
		ampEnv.Release = striker->ampRelease;
		ampEnv.Trigger();
	}

	void Striker::StrikerVoice::NoteOff()
	{
		Voice::NoteOff();
		pitchEnv.Off();
		ampEnv.Off();
	}

	Striker::VariableDelay::VariableDelay(float capacityMs)
	{
		readHead = 0;
		writeHead = 0;
		capacity = Helpers::MsToSamples(capacityMs);
		buffer = new float[capacity];
		for (int i = 0; i < capacity; ++i) buffer[i] = 0.0f;
		SetLengthSamples(capacity);
	}

	Striker::VariableDelay::~VariableDelay()
	{
		delete[] buffer;
	}

	void Striker::VariableDelay::SetLengthMs(float newLengthMs)
	{
		int newLength = Helpers::MsToSamples(newLengthMs);
		SetLengthSamples(newLength);
	}

	void Striker::VariableDelay::SetLengthSamples(int newLength)
	{
		length = newLength;
		if (length < 1) length = 1;
		if (length > capacity) length = capacity;
		readHead = writeHead - length;
		if (readHead < 0) readHead += length;
	}

	void Striker::VariableDelay::Write(float sample)
	{
		writeHead = advanceHead(writeHead);
		buffer[writeHead] = sample;
	}

	float Striker::VariableDelay::Read()
	{
		readHead = advanceHead(readHead);
		return buffer[readHead];
	}

	int Striker::VariableDelay::advanceHead(int head) const
	{
		return (head + 1) % length;
	}

	float Striker::VariableDelay::ProcessAllpass(float input, float gain)
	{
		float out = Read();
		float sum = input + out * gain;
		Write(sum);
		return out - sum * gain;
	}
}
