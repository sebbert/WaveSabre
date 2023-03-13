#define NOMINMAX

#include <WaveSabreCore/Barley.h>
#include <WaveSabreCore/Helpers.h>

#include <clouds/dsp/frame.h>
#include <clouds/dsp/granular_processor.h>

#include <cmath>

namespace WaveSabreCore
{
	Barley::Barley()
		: SynthDevice((int)ParamIndices::NumParams)
	{
		for (int i = 0; i < maxVoices; i++) voices[i] = new BarleyVoice(this);

		buffer = nullptr;
		bufferSize = 0;
		writeHead = 0;
		bufferSizeParam = 0.7f;
		xfadeAmt = 0.1f;
		grainPosition = 0.5f;
		grainPositionSpread = 0.1f;
		grainSize = 0.1f;
		grainSizeSpread = 0.5f;
		probability = 4.0f / Helpers::CurrentSampleRate;
		density = 0.5f;
		stereoSpread = 0.5f;
		nextGrainCountdown = 0;
	}

	Barley::~Barley()
	{
		if (buffer) delete[] buffer;
	}

	void Barley::SetParam(int index, float value)
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::GrainPosition: grainPosition = value; break;
		case ParamIndices::GrainSize: grainSize = value; break;
		case ParamIndices::BufferSize: bufferSizeParam = value; break;
		case ParamIndices::CrossFade: xfadeAmt = value; break;
		case ParamIndices::Probability: probability = value; break;
		case ParamIndices::Density: density = value; break;
		case ParamIndices::StereoSpread: stereoSpread = value; break;
		}
	}

	float Barley::GetParam(int index) const
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::GrainPosition: return grainPosition;
		case ParamIndices::GrainSize: return grainSize;
		case ParamIndices::BufferSize: return bufferSizeParam;
		case ParamIndices::CrossFade: return xfadeAmt;
		case ParamIndices::Probability: return probability;
		case ParamIndices::Density: return density;
		case ParamIndices::StereoSpread: return stereoSpread;
		}
	}

	void Barley::Run(double songPosition, float **inputs, float **outputs, int numSamples)
	{
		auto bufferSizeMs = 20 + 20000.0 * Helpers::Pow2(bufferSizeParam);
		auto newBufferSize = (int)((double)bufferSizeMs * Helpers::CurrentSampleRate / 1000.0);
		if (newBufferSize < 1)
			newBufferSize = 1;

		if (!buffer || bufferSize != newBufferSize)
		{
			if (buffer) delete[] buffer;
			buffer = new float[2 * newBufferSize];
			bufferSize = newBufferSize;
		}

		for (auto i = 0; i < numSamples; ++i)
		{
			buffer[writeHead*2 + 0] = inputs[0][i];
			buffer[writeHead*2 + 1] = inputs[1][i];

			writeHead++;
			if (writeHead >= bufferSize)
				writeHead -= bufferSize;
		}

		xfadeSize = (int)(bufferSize * xfadeAmt);

		activeVoiceCount = 0;

		SynthDevice::Run(songPosition, inputs, outputs, numSamples);
	}

	void Barley::Render(double songPosition, float** inputs, float** outputs, int numSamples)
	{
		SynthDevice::Render(songPosition, inputs, outputs, numSamples);


		// Schedule new grains
		if (activeVoiceCount)
		{
			// Build a list of grains that are not currently running
			freeGrainCount = 0;
			for (auto i = 0; i < MaxGrains; ++i)
			{
				auto& grain = grains[i];
				if (!grain.active)
				{
					freeGrains[freeGrainCount++] = i;
				}
			}

			for (auto i = 0; i < numSamples && freeGrainCount > 0; ++i)
			{
				// if (Helpers::RandFloat() < probability)
				if (--nextGrainCountdown <= 0)
				{
					nextGrainCountdown = (int)(Helpers::CurrentSampleRate / 12.0);

					--freeGrainCount;
					auto grainIndex = freeGrains[freeGrainCount];
					auto grain = &grains[grainIndex];
					ScheduleGrain(grain, i);
				}
			}
		}

		prevFreeGrainCount = freeGrainCount;

		for (auto i = 0; i < MaxGrains; ++i)
		{
			auto grain = &grains[i];
			grain->Run(this, outputs, numSamples);
		}
	}

	static inline double ModEuclid(double x, double y)
	{
		auto r = fmod(x, y);
		if (x < 0) r += y;
		return r;
	}

	void Barley::ScheduleGrain(Grain* grain, int preDelay)
	{
		auto xfadeBufferSize = bufferSize - xfadeSize;
		auto sizeFactor = grainSize;// + grainSizeSpread * Helpers::RandFloat() * 2.0f - 1.0f;
		double size = (bufferSize * sizeFactor);
		if (size < 128) size = 128;
		if (size > bufferSize * 0.6) size = bufferSize * 0.6;

		auto voiceIndex = (int)(Helpers::RandFloat() * activeVoiceCount);
		auto voice = activeVoices[voiceIndex];
		auto freq = Helpers::NoteToFreq(voice->GetNote());

		auto pan = stereoSpread * 2.0f * (Helpers::RandFloat() - 0.5f);

		auto available = (double)bufferSize;
		available -= size*2;
		if (available < 0) return;
		auto startPos = writeHead - available * grainPosition;

		grain->firstSample = (int)ModEuclid(startPos, bufferSize);
		grain->phase = 0.0;
		grain->phaseIncrement = 1.0; //freq;
		grain->size = (int)floor(size);
		grain->active = true;
		grain->preDelay = preDelay;
		grain->envPhase = 0.0f;
		grain->envPhaseIncrement = (float)(2.0 / size);
		grain->envSmooth = 0.5; // TODO: Add parameter
		
		if (pan < 0.0f) {
			grain->gainL = 1.0f;
			grain->gainR = 1.0f + pan;
		} else {
			grain->gainL = 1.0f - pan;
			grain->gainR = 1.0f;
		}
	}

	void Barley::Grain::Run(Barley* device, float** outputs, const int numSamples)
	{
		if (!active) return;

		float frame[2];

		const auto bufferSize = device->bufferSize;
		const auto writeHead = device->writeHead;
		const auto xfadeSize = device->xfadeSize;
		const auto xfadeBufferSize = bufferSize - device->xfadeSize;
		const double invXfadeSize = 1.0 / xfadeSize;
		
		int index = preDelay;
		if (index > numSamples) index = numSamples;
		preDelay = 0;

		for (; index < numSamples; ++index)
		{
			auto position = firstSample + phase;
			double xfadeEnd = writeHead;
			double xfadeStart = xfadeEnd - xfadeSize;
			double samplePos = ModEuclid(position, bufferSize);

			device->ReadFrame(samplePos, frame);
/*
			double adjustedPos = samplePos;
			if (adjustedPos > xfadeEnd)
				adjustedPos -= xfadeBufferSize;
			
			double xfadePosition = adjustedPos - xfadeStart;
			auto xfadePhase = (float)(xfadePosition * invXfadeSize);
			if (xfadePhase >= 0.0f && xfadePhase <= 1.0f)
			{
				float xfadeFrame[2];
				device->ReadFrame((int)ModEuclid(xfadePosition, bufferSize), xfadeFrame);
				
				frame[0] = Helpers::Mix(frame[0], xfadeFrame[0], xfadePhase);
				frame[1] = Helpers::Mix(frame[1], xfadeFrame[1], xfadePhase);
			}
*/

			float env = envPhase;
			if (env >= 1.0f)
				env = 2.0f - env;

/*
			float window = 1.0f - (float)(Helpers::FastCos(env * M_PI) + 1.0f) * 0.5f;
			env += envSmooth * (window - env);
*/

			outputs[0][index] += frame[0] * env * gainL * 0.2f;
			outputs[1][index] += frame[1] * env * gainR * 0.2f;

			phase += phaseIncrement;
			envPhase += envPhaseIncrement;

			if (phase > (double)size)
			{
				active = false;
				return;
			}
		}
	}

	inline void Barley::ReadFrame(double samplePos, float* frame)
	{
		double samplePosFloor = floor(samplePos);
		double samplePosFract = samplePos - samplePosFloor;
		
		int leftIndex = (int)samplePosFloor;
		int rightIndex = leftIndex + 1;
		
		for (auto ch = 0; ch < 2; ++ch)
		{
			float left = buffer[leftIndex * 2 + ch];
			float right = rightIndex < bufferSize ? buffer[rightIndex * 2 + ch] : 0.0f;
			frame[ch] = (float)((double)left * (1.0 - samplePosFract) + (double)right * samplePosFract);
		}
	}

	Barley::BarleyVoice::BarleyVoice(Barley* device)
		: device(device)
	{
	}
	
	SynthDevice *Barley::BarleyVoice::GetSynthDevice() const
	{
		return device;
	}

	void Barley::BarleyVoice::Run(double songPosition, float **outputs, int numSamples)
	{
		device->activeVoices[device->activeVoiceCount++] = this;
	}
}
