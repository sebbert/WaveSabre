#include "StrikerVst.h"
#include "StrikerEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
	Helpers::Init();
	return new StrikerVst(audioMaster);
}

StrikerVst::StrikerVst(audioMasterCallback audioMaster)
	: VstPlug(audioMaster, (int)Striker::ParamIndices::NumParams, 0, 2, 'Strk', new Striker(), true)
{
	setEditor(new StrikerEditor(this));
}

void StrikerVst::getParameterName(VstInt32 index, char *text)
{
	switch ((Striker::ParamIndices)index)
	{
	case Striker::ParamIndices::NoiseImpulseLevel: vst_strncpy(text, "NsImpLv", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::SineImpulseLevel: vst_strncpy(text, "SinImpLv", kVstMaxParamStrLen); break;
	
	case Striker::ParamIndices::CombFeedback: vst_strncpy(text, "Comb FB", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::AllpassGain: vst_strncpy(text, "AP Gain", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::Damping: vst_strncpy(text, "Damping", kVstMaxParamStrLen); break;

	case Striker::ParamIndices::PitchAttack: vst_strncpy(text, "P Atk", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::PitchDecay: vst_strncpy(text, "P Dcy", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::PitchSustain: vst_strncpy(text, "P Sus", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::PitchRelease: vst_strncpy(text, "P Rel", kVstMaxParamStrLen); break;

	case Striker::ParamIndices::AmpAttack: vst_strncpy(text, "A Atk", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::AmpDecay: vst_strncpy(text, "A Dcy", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::AmpSustain: vst_strncpy(text, "A Sus", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::AmpRelease: vst_strncpy(text, "A Rel", kVstMaxParamStrLen); break;

	default: break;
	}
}

bool StrikerVst::getEffectName(char *name)
{
	vst_strncpy(name, "WaveSabre - Striker", kVstMaxEffectNameLen);
	return true;
}

bool StrikerVst::getProductString(char *text)
{
	vst_strncpy(text, "WaveSabre - Striker", kVstMaxProductStrLen);
	return true;
}
