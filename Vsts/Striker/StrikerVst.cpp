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
	case Striker::ParamIndices::NoiseImpactLevel: vst_strncpy(text, "NsImpLvl", kVstMaxParamStrLen); break;
	case Striker::ParamIndices::SineImpactLevel: vst_strncpy(text, "SnImpLvl", kVstMaxParamStrLen); break;
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
