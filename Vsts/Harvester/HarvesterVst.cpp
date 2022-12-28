#include "HarvesterVst.h"
#include "HarvesterEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
	Helpers::Init();
	return new HarvesterVst(audioMaster);
}

HarvesterVst::HarvesterVst(audioMasterCallback audioMaster)
	: VstPlug(audioMaster, (int)Harvester::ParamIndices::NumParams, 2, 2, 'Hvst', new Harvester())
{
	setEditor(new HarvesterEditor(this));
}

static const char* getParameterNameImpl(Harvester::ParamIndices index)
{
	switch (index)
	{
		case Harvester::ParamIndices::Position:     return "Position";
		case Harvester::ParamIndices::Size:         return "Size";
		case Harvester::ParamIndices::StereoSpread: return "Stereo spread";
		case Harvester::ParamIndices::Reverb:       return "Reverb";
		case Harvester::ParamIndices::DryWet:       return "Dry/Wet";
		default: return "???";
	}
}

void HarvesterVst::getParameterName(VstInt32 index, char *text)
{
	vst_strncpy(text, getParameterNameImpl((Harvester::ParamIndices)index), kVstMaxParamStrLen);
}

bool HarvesterVst::getEffectName(char *name)
{
	vst_strncpy(name, "WaveSabre - Harvester", kVstMaxEffectNameLen);
	return true;
}

bool HarvesterVst::getProductString(char *text)
{
	vst_strncpy(text, "WaveSabre - Harvester", kVstMaxProductStrLen);
	return true;
}
