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
		writeHead = 0;
		bufferSizeParam = 0.5f;
		xfadeAmt = 0.4f;
		grainPosition = 0.0f;
		grainPositionSpread = 0.1f;
		grainSize = 0.5f;
		grainSizeSpread = 0.2f;
		probability = 0.5f;
		density = 0.5f;
		stereoSpread = 0.5f;
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

		xfadeSize = (int)(bufferSize - bufferSize * xfadeAmt);

		activeVoiceCount = 0;
		SynthDevice::Run(songPosition, inputs, outputs, numSamples);
	}

	void Barley::Render(double songPosition, float** inputs, float** outputs, int numSamples)
	{
		SynthDevice::Render(songPosition, inputs, outputs, numSamples);

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

		// Schedule new grains
		for (auto i = 0; freeGrainCount && i < numSamples; ++i)
		{
			if (Helpers::RandFloat() < probability)
			{
				--freeGrainCount;
				auto grainIndex = freeGrains[freeGrainCount];
				auto grain = &grains[grainIndex];
				ScheduleGrain(grain, i);
			}
		}

		for (auto i = 0; i < MaxGrains; ++i)
		{
			auto grain = &grains[i];
			grain->Run(this, outputs, numSamples);
		}
	}

	void Barley::ScheduleGrain(Grain* grain, int preDelay)
	{
		auto sizeFactor = grainSize + grainSizeSpread * Helpers::RandFloat() * 2.0f - 1.0f;
		auto size = (int)(bufferSize * sizeFactor);
		if (size < 128) size = 128;
		if (size > bufferSize) size = bufferSize;

		auto voiceIndex = (int)(Helpers::RandFloat() * activeVoiceCount);
		auto voice = activeVoices[voiceIndex];
		auto freq = Helpers::NoteToFreq(voice->GetNote());

		auto pan = stereoSpread * 2.0f * (Helpers::RandFloat() - 0.5f);

		grain->position = writeHead - grainPosition * bufferSize;
		grain->positionIncrement = freq;
		grain->size = size;
		grain->active = true;
		grain->preDelay = preDelay;
		grain->position = 0.0f;
		
		if (pan < 0.0f) {
			grain->gainL = 1.0f;
			grain->gainR = 1.0f + pan;
		} else {
			grain->gainL = 1.0f - pan;
			grain->gainR = 1.0f;
		}
	}

	static inline double ModEuclid(double x, double n)
	{
		auto r = fmod(x, n);
		if (x < 0) r += n;
		return r;
	}

	void Barley::Grain::Run(Barley* device, float** outputs, int numSamples)
	{
		if (!active) return;

		auto outputL = outputs[0];
		auto outputR = outputs[1];
		auto remaining = numSamples;

		while (preDelay && remaining)
		{
			++outputL;
			++outputR;
			--remaining;
			--preDelay;
		}

		auto bufferSize = device->bufferSize;
		auto writeHead = device->writeHead;
		auto xfadeSize = device->xfadeSize;
		auto xfadeBufferSize = bufferSize - device->xfadeSize;
		double invXfadeSize = 1.0 / xfadeSize;

		float frame[2];

		while (remaining--)
		{
			double xfadeEnd = writeHead;
			double xfadeStart = xfadeEnd - xfadeSize;
			double samplePos = ModEuclid(position, xfadeBufferSize);

			device->ReadFrame(samplePos, frame);

			double adjustedPos = samplePos;
			if (adjustedPos > xfadeEnd)
				adjustedPos -= xfadeBufferSize;
			
			double phase = (adjustedPos - xfadeStart) * invXfadeSize;
			if (phase >= 0.0 && phase <= 1.0)
			{
				float xfadeFrame[2];
				device->ReadFrame()
			}

			if (position )

			*(++outputL) = frame[0];
			*(++outputR) = frame[1];


			position += positionIncrement;
		}
	}

	void Barley::ReadFrame(double samplePos, float* frame)
	{
		auto samplePosFloor = floor(samplePos);
		auto samplePosFract = samplePos - samplePosFloor;
		
		auto leftIndex = (int)samplePosFloor;
		int rightIndex = leftIndex + 1;
		
		for (auto ch = 0; ch < 2; ++ch)
		{
			auto left = buffer[leftIndex*2 + ch];
			auto right = rightIndex < bufferSize ? buffer[rightIndex*2 + ch] : 0.0f;
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

	void Barley::BarleyVoice::NoteOn(int note, int velocity, float detune, float pan)
	{
	}

	void Barley::BarleyVoice::NoteOff()
	{
	}
}
