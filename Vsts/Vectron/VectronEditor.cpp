#include "VectronEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

VectronEditor::VectronEditor(AudioEffect *audioEffect)
	: VstEditor(audioEffect, 800, 600, "VECTRON")
{
}

VectronEditor::~VectronEditor()
{
}

void VectronEditor::Open()
{
	addKnob((VstInt32)Vectron::ParamIndices::Osc1X, "X");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1XEnv1Amt, "ENV1 X");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1XEnv2Amt, "ENV2 X");

	startNextRow();

	addKnob((VstInt32)Vectron::ParamIndices::Osc1Y, "Y");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1YEnv1Amt, "ENV1 Y");
	addKnob((VstInt32)Vectron::ParamIndices::Osc1YEnv2Amt, "ENV2 Y");

	startNextRow();

	addKnob((VstInt32)Vectron::ParamIndices::Gain, "GAIN");
	addKnob((VstInt32)Vectron::ParamIndices::GainEnv1Amt, "ENV1 GAIN");
	addKnob((VstInt32)Vectron::ParamIndices::GainEnv2Amt, "ENV2 GAIN");

	startNextRow();
	startNextRow();

	addKnob((VstInt32)Vectron::ParamIndices::Env1Attack, "ENV1 ATK");
	addKnob((VstInt32)Vectron::ParamIndices::Env1Decay, "ENV1 DCY");
	addKnob((VstInt32)Vectron::ParamIndices::Env1Sustain, "ENV1 SUS");
	addKnob((VstInt32)Vectron::ParamIndices::Env1Release, "ENV1 RLS");

	startNextRow();

	addKnob((VstInt32)Vectron::ParamIndices::Env2Attack, "ENV2 ATK");
	addKnob((VstInt32)Vectron::ParamIndices::Env2Decay, "ENV2 DCY");
	addKnob((VstInt32)Vectron::ParamIndices::Env2Sustain, "ENV2 SUS");
	addKnob((VstInt32)Vectron::ParamIndices::Env2Release, "ENV2 RLS");

	startNextRow();

	VstEditor::Open();
}
