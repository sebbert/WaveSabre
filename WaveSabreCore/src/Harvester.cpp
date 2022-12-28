#define NOMINMAX

#include <WaveSabreCore/Harvester.h>
#include <WaveSabreCore/Helpers.h>

#include <clouds/dsp/frame.h>
#include <clouds/dsp/granular_processor.h>

#include <cmath>

namespace WaveSabreCore
{
	Harvester::Harvester()
		: Device((int)ParamIndices::NumParams)
		, buffer_large{0}
		, buffer_small{0}
		, processor{new clouds::GranularProcessor()}
	{
		memset(buffer_large, 0, sizeof(buffer_large));
		memset(buffer_small, 0, sizeof(buffer_small));

		processor->Init(
			buffer_large, sizeof(buffer_large),
			buffer_small, sizeof(buffer_small)
		);

		processor->set_playback_mode(clouds::PLAYBACK_MODE_GRANULAR);
		processor->set_low_fidelity(false);
		processor->set_num_channels(2);

		auto params = processor->mutable_parameters();
		params->dry_wet = 0.5f;
		params->pitch = 0.0f;
		params->density = 0.9f;
		params->feedback = 0.0f;
		params->reverb = 0.5f;
		params->trigger = false;
		params->freeze = false;
		params->gate = true;
		params->texture = 0.5;

		processor->Prepare();
		processor->set_silence(false);
	}

	Harvester::~Harvester()
	{
		if (processor)
		{
			delete processor;
			processor = nullptr;
		}
	}

	void Harvester::Run(double songPosition, float **inputs, float **outputs, int numSamples)
	{
		constexpr float INT16_SCALE_FACTOR = 32768.0f;

		// TODO: Modify GranularProcessor to accept float samples (it converts internally anyway)
		clouds::ShortFrame tmpIn[clouds::kMaxBlockSize];
		clouds::ShortFrame tmpOut[clouds::kMaxBlockSize];

		int blockOffset = 0;
		int remainingSamples = numSamples;
		while (remainingSamples)
		{
			int blockSize = std::min(remainingSamples, (int)clouds::kMaxBlockSize);
			for (int i = 0; i < blockSize; ++i)
			{
				tmpIn[i].l = static_cast<short>(inputs[0][blockOffset + i] * INT16_SCALE_FACTOR);
				tmpIn[i].r = static_cast<short>(inputs[1][blockOffset + i] * INT16_SCALE_FACTOR);
			}

			processor->Prepare();
			processor->Process(tmpIn, tmpOut, blockSize);

			for (int i = 0; i < blockSize; ++i)
			{
				outputs[0][blockOffset + i] = static_cast<float>(tmpOut[i].l) / INT16_SCALE_FACTOR;
				outputs[1][blockOffset + i] = static_cast<float>(tmpOut[i].r) / INT16_SCALE_FACTOR;
			}

			blockOffset += blockSize;
			remainingSamples -= blockSize;
		}
	}

	void Harvester::SetParam(int index, float value)
	{
		auto params = processor->mutable_parameters();
		switch ((ParamIndices)index)
		{
		case ParamIndices::Position: params->position = value; break;
		case ParamIndices::Size: params->size = value; break;
		case ParamIndices::StereoSpread: params->stereo_spread = value; break;
		case ParamIndices::Reverb: params->reverb = value; break;
		case ParamIndices::DryWet: params->dry_wet = value; break;
		default: break;
		}
	}

	float Harvester::GetParam(int index) const
	{
		auto& params = processor->parameters();
		switch ((ParamIndices)index)
		{
		case ParamIndices::Position: return params.position;
		case ParamIndices::Size: return params.size;
		case ParamIndices::StereoSpread: return params.stereo_spread;
		case ParamIndices::Reverb: return params.reverb;
		case ParamIndices::DryWet: return params.dry_wet;
		default: return 0.0;
		}
	}
}
