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
	}

	void Striker::SetParam(int index, float value)
	{
		switch ((ParamIndices)index)
		{
			case ParamIndices::NoiseImpactLevel: noiseImpactLevel = value;
			case ParamIndices::SineImpactLevel: sineImpactLevel = value;
		}
	}

	float Striker::GetParam(int index) const
	{
		switch ((ParamIndices)index)
		{
			case ParamIndices::NoiseImpactLevel: return noiseImpactLevel;
			case ParamIndices::SineImpactLevel: return sineImpactLevel;
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

			double freq = Helpers::NoteToFreq(Note);


			comb.SetLengthSamples(waveLengthSamples);
			float combOut = comb.Read();
			float combIn = impulse + combOut * 0.95;
			comb.Write(combIn);

			outputs[0][sampleIndex] += combOut;
			outputs[1][sampleIndex] += combOut;

			currentSamples += 1;
		}
	}

	void Striker::StrikerVoice::NoteOn(int note, int velocity, float detune, float pan)
	{
		Voice::NoteOn(note, velocity, detune, pan);
		double freq = Helpers::NoteToFreq(GetNote());
		waveLengthSamples = (int)(Helpers::CurrentSampleRate / freq);
		currentSamples = 0;
		noise.Reset();
	}

	void Striker::StrikerVoice::NoteOff()
	{
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
