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
	case Vectron::ParamIndices::Osc1Y: return "O1 Y";

	case Vectron::ParamIndices::Osc1AmpAttack: return "O1 Amp A";
	case Vectron::ParamIndices::Osc1AmpDecay: return "O1 Amp D";
	case Vectron::ParamIndices::Osc1AmpSustain: return "O1 Amp S";
	case Vectron::ParamIndices::Osc1AmpRelease: return "O1 Amp R";

	case Vectron::ParamIndices::Osc1YAttack: return "O1 Y A";
	case Vectron::ParamIndices::Osc1YDecay: return "O1 Y D";
	case Vectron::ParamIndices::Osc1YSustain: return "O1 Y S";
	case Vectron::ParamIndices::Osc1YRelease: return "O1 Y R";

	case Vectron::ParamIndices::Osc1YEnvAmt: return "O1 Y Amt";

	case Vectron::ParamIndices::MasterLevel: return "Mstr Lvl";

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
