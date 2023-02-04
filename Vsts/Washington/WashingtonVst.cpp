#include "WashingtonVst.h"
#include "WashingtonEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
	Helpers::Init();
	return new WashingtonVst(audioMaster);
}

WashingtonVst::WashingtonVst(audioMasterCallback audioMaster)
	: VstPlug(audioMaster, (int)Washington::ParamIndices::NumParams, 0, 2, 'D.C.', new Washington(), true)
{
	setEditor(new WashingtonEditor(this));
}

void WashingtonVst::getParameterName(VstInt32 index, char *text)
{
	switch ((Washington::ParamIndices)index)
	{
	case Washington::ParamIndices::Attack: vst_strncpy(text, "Attack", kVstMaxParamStrLen); break;
	case Washington::ParamIndices::Decay: vst_strncpy(text, "Decay", kVstMaxParamStrLen); break;
	case Washington::ParamIndices::Sustain: vst_strncpy(text, "Sustain", kVstMaxParamStrLen); break;
	case Washington::ParamIndices::Release: vst_strncpy(text, "Release", kVstMaxParamStrLen); break;
	case Washington::ParamIndices::KeyMod: vst_strncpy(text, "Key Mod", kVstMaxParamStrLen); break;
	case Washington::ParamIndices::Gain: vst_strncpy(text, "Gain", kVstMaxParamStrLen); break;
	}
}

bool WashingtonVst::getEffectName(char *name)
{
	vst_strncpy(name, "WaveSabre - Washington", kVstMaxEffectNameLen);
	return true;
}

bool WashingtonVst::getProductString(char *text)
{
	vst_strncpy(text, "WaveSabre - Washington", kVstMaxProductStrLen);
	return true;
}
