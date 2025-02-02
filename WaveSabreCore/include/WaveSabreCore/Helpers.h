#ifndef __WAVESABRECORE_HELPERS_H__
#define __WAVESABRECORE_HELPERS_H__

#include "StateVariableFilter.h"
#include "Twister.h"
#include "SynthDevice.h"
#include "Random.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define M_LN20 2.995732273553991

namespace WaveSabreCore
{
	class Helpers
	{
	public:
		static double CurrentSampleRate;
		static int CurrentTempo;
		static Random GlobalRandom;

		static void Init();

		static inline float RandFloat()
		{
			return GlobalRandom.Next();
		}

		static double Exp2(double x);
		static float Exp2F(float x);

		static inline float Exp10F(float x)
		{
			const float scale = (float)(M_LN10 / M_LN2);
			return Exp2F(x * scale);
		}

		static inline float Exp20F(float x)
		{
			const float scale = (float)(M_LN20 / M_LN2);
			return Exp2F(x * scale);
		}

		static inline double Pow2(double x)
		{
			return x * x;
		}

		static inline double Pow4(double x)
		{
			return Pow2(Pow2(x));
		}

		static inline float Pow2F(float x)
		{
			return x * x;
		}

		static inline double UnitToSigned(double x)
		{
			return x * 2.0 - 1.0;
		}

		static inline double SignedToUnit(double x)
		{
			return x * 0.5 + 0.5;
		}

		static inline float UnitToSigned(float x)
		{
			return x * 2.0f - 1.0f;
		}

		static inline float SignedToUnit(float x)
		{
			return x * 0.5f + 0.5f;
		}

		static double FastCos(double x);
		static double FastSin(double x);

		static float FastTanh(float x);

		static double NoteToFreq(double note);

		static double Square135(double phase);
		static double Square35(double phase);

		static float Mix(float v1, float v2, float mix);
		static float Clamp(float f, float min, float max);

		static float DbToScalar(float db);

		static float EnvValueToScalar(float value);
		static float ScalarToEnvValue(float scalar);

		static float VolumeToScalar(float volume);
		static float ScalarToVolume(float scalar);

		static bool ParamToBoolean(float value);
		static float BooleanToParam(bool b);

		static float ParamToFrequency(float param);
		static float FrequencyToParam(float freq);

		static float ParamToQ(float param);
		static float QToParam(float q);

		static float ParamToDb(float param, float range = 24.0f);
		static float DbToParam(float db, float range = 24.0f);

		static float ParamToResonance(float param);
		static float ResonanceToParam(float resonance);

		static StateVariableFilterType ParamToStateVariableFilterType(float param);
		static float StateVariableFilterTypeToParam(StateVariableFilterType type);

		static int ParamToUnisono(float param);
		static float UnisonoToParam(int unisono);

		static double ParamToVibratoFreq(float param);
		static float VibratoFreqToParam(double vf);

		static float PanToScalarLeft(float pan);
		static float PanToScalarRight(float pan);

		static Spread ParamToSpread(float param);
		static float SpreadToParam(Spread spread);

		static VoiceMode ParamToVoiceMode(float param);
		static float VoiceModeToParam(VoiceMode type);

		static int MsToSamples(double ms);

		static double FreqToWaveLengthSamples(double freq);
	private:
		static const int fastSinTabLog2Size = 9; // size = 512
		static const int fastSinTabSize = (1 << fastSinTabLog2Size);
		static const int adjustedFastSinTabSize = fastSinTabSize + 1;
		static double fastSinTab[adjustedFastSinTabSize];
	};
}

#endif
