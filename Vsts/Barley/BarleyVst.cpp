#include "BarleyVst.h"
#include "BarleyEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
	Helpers::Init();
	return new BarleyVst(audioMaster);
}

BarleyVst::BarleyVst(audioMasterCallback audioMaster)
	: VstPlug(audioMaster, (int)Barley::ParamIndices::NumParams, 2, 2, 'Brly', new Barley())
{
	setEditor(new BarleyEditor(this));
}

static const char* getParameterNameImpl(Barley::ParamIndices index)
{
	switch (index)
	{
		case Barley::ParamIndices::GrainPosition:       return "Position";
		case Barley::ParamIndices::GrainPositionSpread: return "PosSprd";
		case Barley::ParamIndices::GrainSize:           return "Size";
		case Barley::ParamIndices::GrainSizeSpread:     return "SizeSprd";
		case Barley::ParamIndices::BufferSize:          return "BufSize";
		case Barley::ParamIndices::CrossFade:           return "Xfade";
		case Barley::ParamIndices::Probability:         return "Probably";
		case Barley::ParamIndices::Density:             return "Density";
		default: return "???";
	}
}

void BarleyVst::getParameterName(VstInt32 index, char *text)
{
	vst_strncpy(text, getParameterNameImpl((Barley::ParamIndices)index), kVstMaxParamStrLen);
}


bool BarleyVst::canParameterBeAutomated(VstInt32 index)
{
	switch ((Barley::ParamIndices)index)
	{
		case Barley::ParamIndices::BufferSize:
			return false;

		default:
			return true;
	}
}

bool BarleyVst::getEffectName(char *name)
{
	vst_strncpy(name, "WaveSabre - Barley", kVstMaxEffectNameLen);
	return true;
}

bool BarleyVst::getProductString(char *text)
{
	vst_strncpy(text, "WaveSabre - Barley", kVstMaxProductStrLen);
	return true;
}
