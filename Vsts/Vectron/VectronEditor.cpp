#include "VectronEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

VectronEditor::VectronEditor(AudioEffect *audioEffect)
	: VstEditor(audioEffect, 820, 220, "VECTRON")
{
}

VectronEditor::~VectronEditor()
{
}

void VectronEditor::Open()
{
	addKnob((VstInt32)Vectron::ParamIndices::Osc1X, "X");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1Y, "Y");

	addSpacer();
	addSpacer();

	addKnob((VstInt32)Vectron::ParamIndices::Osc1YEnvAmt, "Y ENV");

	addSpacer();

	addKnob((VstInt32)Vectron::ParamIndices::Osc1YAttack, "Y ATK");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1YDecay, "Y DCY");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1YSustain, "Y SUS");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1YRelease, "Y RLS");

	addSpacer();
	addSpacer();

	addKnob((VstInt32)Vectron::ParamIndices::Osc1AmpAttack, "AMP ATK");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1AmpDecay, "AMP DCY");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1AmpSustain, "AMP SUS");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1AmpRelease, "AMP RLS");

	startNextRow();

	addKnob((VstInt32)Vectron::ParamIndices::MasterLevel, "MASTER");

	VstEditor::Open();
}
