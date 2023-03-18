#include "VectronVst.h"
#include "VectronEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
	Helpers::Init();
	return new VectronVst(audioMaster);
}

VectronVst::VectronVst(audioMasterCallback audioMaster)
	: VstPlug(audioMaster, (int)Vectron::ParamIndices::NumParams, 0, 2, 'Vtrn', new Vectron(), true)
{
	setEditor(new VectronEditor(this));
}


static const char* getParameterNameImpl(Vectron::ParamIndices index)
{
	switch (index)
	{
	case Vectron::ParamIndices::Osc1X: return "O1 X";
	case Vectron::ParamIndices::Osc1XEnv1Amt: return "O1 X E1";
	case Vectron::ParamIndices::Osc1XEnv2Amt: return "O1 X E2";
	
	case Vectron::ParamIndices::Osc1Y: return "O1 Y";
	case Vectron::ParamIndices::Osc1YEnv1Amt: return "O1 Y E1";
	case Vectron::ParamIndices::Osc1YEnv2Amt: return "O1 Y E2";

	case Vectron::ParamIndices::Gain: return "Gain";
	case Vectron::ParamIndices::GainEnv1Amt: return "Gain E1";
	case Vectron::ParamIndices::GainEnv2Amt: return "Gain E2";

	case Vectron::ParamIndices::Env1Attack: return "E1 Atk";
	case Vectron::ParamIndices::Env1Decay: return "E1 Dcy";
	case Vectron::ParamIndices::Env1Sustain: return "E1 Sus";
	case Vectron::ParamIndices::Env1Release: return "E1 Rls";

	case Vectron::ParamIndices::Env2Attack: return "E2 Atk";
	case Vectron::ParamIndices::Env2Decay: return "E2 Dcy";
	case Vectron::ParamIndices::Env2Sustain: return "E2 Sus";
	case Vectron::ParamIndices::Env2Release: return "E2 Rls";

	default: return "???";
	}
}

void VectronVst::getParameterName(VstInt32 index, char *text)
{
	auto name = getParameterNameImpl((Vectron::ParamIndices)index);
	vst_strncpy(text, name, kVstMaxParamStrLen);
}

bool VectronVst::getEffectName(char *name)
{
	vst_strncpy(name, "WaveSabre - Vectron", kVstMaxEffectNameLen);
	return true;
}

bool VectronVst::getProductString(char *text)
{
	vst_strncpy(text, "WaveSabre - Vectron", kVstMaxProductStrLen);
	return true;
}
