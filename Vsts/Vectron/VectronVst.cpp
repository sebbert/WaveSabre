#include "VectronVst.h"
#include "VectronEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;


#define GET_MOD_PARAM_NAME(INDEX, NAME)  \
	case INDEX:          return (NAME); \
	case INDEX##Env1Amt: return (NAME "E1"); \
	case INDEX##Env2Amt: return (NAME "E2");


AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
	Helpers::Init();
	return new VectronVst(audioMaster);
}

VectronVst::VectronVst(audioMasterCallback audioMaster)
	: VstPlug(audioMaster, (int)Vectron::ParamIndices::NumParams, 0, 2, 'Vtrn', new Vectron(), true)
	, Device(static_cast<Vectron*>(getDevice()))
{
	setEditor(new VectronEditor(this));
}

static const char* getParameterNameImpl(Vectron::ParamIndices index)
{
	switch (index)
	{
	GET_MOD_PARAM_NAME(Vectron::ParamIndices::ModScale, "PmScl")
	GET_MOD_PARAM_NAME(Vectron::ParamIndices::ModOffset, "PmOfs")
	
	GET_MOD_PARAM_NAME(Vectron::ParamIndices::ModXScale, "PmXScl")
	GET_MOD_PARAM_NAME(Vectron::ParamIndices::ModXOffset, "PmXOfs")
	GET_MOD_PARAM_NAME(Vectron::ParamIndices::ModXDetune, "PmXDet")
	
	GET_MOD_PARAM_NAME(Vectron::ParamIndices::ModYScale, "PmYScl")
	GET_MOD_PARAM_NAME(Vectron::ParamIndices::ModYOffset, "PmYOfs")
	GET_MOD_PARAM_NAME(Vectron::ParamIndices::ModYDetune, "PmYDet")

	GET_MOD_PARAM_NAME(Vectron::ParamIndices::Osc1Offset, "O1Ofs")
	GET_MOD_PARAM_NAME(Vectron::ParamIndices::Osc1Mod, "O1Mod")

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
