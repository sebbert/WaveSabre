#pragma once

#include "SynthDevice.h"
#include "Envelope.h"
#include "StateVariableFilter.h"

namespace WaveSabreCore
{
	class Washington : public SynthDevice
	{
	public:
		enum class ParamIndices
		{
			Attack,
			Decay,
			Sustain,
			Release,

			KeyMod,

			Gain,

			NumParams,
		};

		Washington();

		virtual void SetParam(int index, float value);
		virtual float GetParam(int index) const;

	protected:
		class WashingtonVoice : public Voice
		{
		public:
			WashingtonVoice(Washington *washington);
			virtual WaveSabreCore::SynthDevice *GetSynthDevice() const;

			virtual void Run(double songPosition, float **outputs, int numSamples);

			virtual void NoteOn(int note, int velocity, float detune, float pan);
			virtual void NoteOff();

		private:
			Washington *washington;

			Envelope env;
		};

		float attack, decay, sustain, release;
		float keyMod;
		float gain;
	};
}
