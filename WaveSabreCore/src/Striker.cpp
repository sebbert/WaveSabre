#include <WaveSabreCore/Striker.h>
#include <WaveSabreCore/Helpers.h>

#include <math.h>

namespace WaveSabreCore
{
	Striker::Striker()
		: SynthDevice((int)ParamIndices::NumParams)
	{
		for (int i = 0; i < maxVoices; i++) voices[i] = new StrikerVoice(this);

		noiseImpulseLevel = 0.5;
		sineImpulseLevel = 0.5;

		feedback = 0.98f;
		damping = 0.5f;

		allpassGain = 0.5f;
		allpassFreq = 0.0;
		allpassRatioCoarse = 2.0;
		allpassRatioFine = 0.5;

		ampAttack = 1.0;
		ampDecay = Helpers::ScalarToEnvValue(0.5);
		ampSustain = 1.0;
		ampRelease = 100.0;

		pitchAttack = 1.0;
		pitchDecay = 30.0;
		pitchSustain = 0.0;
		pitchRelease = 1.0;
	}

	void Striker::SetParam(int index, float value)
	{
		switch ((ParamIndices)index)
		{
			case ParamIndices::NoiseImpulseLevel: noiseImpulseLevel = value; break;
			case ParamIndices::SineImpulseLevel: sineImpulseLevel = value; break;

			case ParamIndices::Feedback: feedback = 1.0f - (float)Helpers::Pow4(1.0 - (double)value); break;
			case ParamIndices::Damping: damping = value; break;

			case ParamIndices::AllpassGain: allpassGain = value; break;
			case ParamIndices::AllpassFreq: allpassFreq = Helpers::ParamToFrequency(value) - 20.0f; break;
			case ParamIndices::AllpassRatioCoarse: allpassRatioCoarse = floorf(value * maxAllpassRatioCoarse); break;
			case ParamIndices::AllpassRatioFine: allpassRatioFine = value; break;

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
			case ParamIndices::NoiseImpulseLevel: return noiseImpulseLevel;
			case ParamIndices::SineImpulseLevel: return sineImpulseLevel;

			case ParamIndices::Feedback: return 1.0f - (float)sqrt(sqrt(1.0 - feedback));
			case ParamIndices::Damping: return damping;

			case ParamIndices::AllpassGain: return allpassGain;
			case ParamIndices::AllpassFreq: return Helpers::FrequencyToParam(allpassFreq + 20.0f);
			case ParamIndices::AllpassRatioCoarse: return allpassRatioCoarse / maxAllpassRatioCoarse;
			case ParamIndices::AllpassRatioFine: return allpassRatioFine;

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
		, combDelay(1000.0)
		, allpassDelay(1000.0)
		, noise()
	{
		combHighpassFilter.SetFreq(20.0);
	}

	SynthDevice *Striker::StrikerVoice::GetSynthDevice() const
	{
		return striker;
	}

	void Striker::StrikerVoice::Run(double songPosition, float **outputs, int numSamples)
	{
		for (int i = 0; i < numSamples; ++i, ++elapsedSamples)
		{
			float noiseImpulse = 0.0f, sineImpulse = 0.0f, impulse = 0.0f;
			if (elapsedSamples <= waveLengthSamples)
			{
				double t = (double)elapsedSamples / (double)waveLengthSamples;
				noiseImpulse = striker->sineImpulseLevel * (float)Helpers::FastSin(t * 2.0 * M_PI);
				sineImpulse = striker->noiseImpulseLevel * noise.Next();
				impulse = noiseImpulse + sineImpulse;
			}

			double note = GetNote();
			double baseFreq = Helpers::NoteToFreq(note);

			pitchEnv.Next();
			float pitchEnvValue = pitchEnv.GetValue();

			// Envelope already squares the decay and release slopes, so these are really 10 and 20
			float pitchEnvPow5 = pitchEnvValue * pitchEnvValue * pitchEnvValue * pitchEnvValue * pitchEnvValue;
			float pitchEnvPow10 = pitchEnvPow5 * pitchEnvPow5;

			float velocityGain = Helpers::Exp2F(-24.0f * (1.0f - velocity) / 6.0f);

			double combFreq = baseFreq * (1.0 + (double)(pitchEnvPow5*(velocity+2.0)));
			combDelay.SetLengthSamples((int)Helpers::FreqToWaveLengthSamples(combFreq));

			double allpassFineBase = Helpers::Pow2(((double)striker->allpassRatioFine - .5) * 2.0);
			double allpassRatio = floor((double)striker->allpassRatioCoarse) + allpassFineBase * allpassFineBase * allpassFineBase;
			double allpassBaseFreq = striker->allpassFreq + baseFreq * allpassRatio;
			double allpassFreq = allpassBaseFreq * (double)(2.0 + pitchEnvPow10 * velocity);
			allpassDelay.SetLengthSamples((int)Helpers::FreqToWaveLengthSamples(allpassFreq));

			float combDelayOut = combDelay.Read();
			float combHighpassOut = combHighpassFilter.ProcessHighpass(combDelayOut);
			float allpassIn = impulse + Helpers::FastTanh(combHighpassOut * striker->feedback);
			float allpassOut = allpassDelay.ProcessAllpass(allpassIn, striker->allpassGain);
			float dampFilterOut = dampFilter.ProcessLowpass(allpassOut);
			combDelay.Write(dampFilterOut);
			float resonatorOut = Helpers::FastTanh(dampFilterOut);// + noiseImpulse * velocityGain * 2.0);

			ampEnv.Next();
			float masterOut = resonatorOut * ampEnv.GetValue();

			outputs[0][i] += masterOut;
			outputs[1][i] += masterOut;
		}

		if (ampEnv.State == EnvelopeState::Finished)
		{
			IsOn = false;
		}
	}

	void Striker::StrikerVoice::NoteOn(int note, int velocityInt, float detune, float pan)
	{
		Voice::NoteOn(note, velocityInt, detune, pan);
		
		velocity = Helpers::Clamp((float)velocityInt / 127.0f, 0.0f, 1.0f);
		
		double baseFreq = Helpers::NoteToFreq(GetNote());
		waveLengthSamples = (int)Helpers::FreqToWaveLengthSamples(baseFreq);
		elapsedSamples = 0;
		
		noise.Reset();

		combDelay.Clear();

		dampFilter.SetCoef(striker->damping);

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

	float Striker::OnePoleFilter::ProcessLowpass(float input)
	{
		state = Helpers::Mix(input, state, coef);
		return state;
	}

	float Striker::OnePoleFilter::ProcessHighpass(float input)
	{
		return input - ProcessLowpass(input);
	}

	void Striker::OnePoleFilter::SetFreq(double freq)
	{
		SetCoef((float)FreqToCoef(freq));
	}

	void Striker::OnePoleFilter::SetCoef(float newCoef)
	{
		coef = Helpers::Clamp(newCoef, 0.0f, 1.0f);
	}

	float Striker::OnePoleFilter::FreqToCoef(double freq)
	{
		constexpr double LOG2_E = 1.4426950408889634;
		return (float)Helpers::Exp2(LOG2_E * 2.0 * M_PI * freq / Helpers::CurrentSampleRate);
	}

	Striker::VariableDelay::VariableDelay(float capacityMs)
	{
		readHead = 0;
		writeHead = 0;
		capacity = Helpers::MsToSamples(capacityMs);
		buffer = new float[capacity];
		Clear();
		SetLengthSamples(capacity);
	}

	Striker::VariableDelay::~VariableDelay()
	{
		delete[] buffer;
	}

	void Striker::VariableDelay::Clear()
	{
		for (int i = 0; i < capacity; ++i) buffer[i] = 0.0f;
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
		buffer[writeHead] = sample;
		writeHead = advanceHead(writeHead);
	}

	float Striker::VariableDelay::Read()
	{
		float sample = buffer[readHead];
		readHead = advanceHead(readHead);
		return sample;
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
