#include "WashingtonEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

WashingtonEditor::WashingtonEditor(AudioEffect *audioEffect)
	: VstEditor(audioEffect, 320, 160, "WASHINGTON")
{
}

WashingtonEditor::~WashingtonEditor()
{
}

void WashingtonEditor::Open()
{
	addKnob((VstInt32)Washington::ParamIndices::Attack, "ATTACK");
	addSpacer();
	addKnob((VstInt32)Washington::ParamIndices::Decay, "DECAY");
	addSpacer();
	addKnob((VstInt32)Washington::ParamIndices::Sustain, "SUSTAIN");
	addSpacer();
	addKnob((VstInt32)Washington::ParamIndices::Release, "RELEASE");

	startNextRow();

	addKnob((VstInt32)Washington::ParamIndices::KeyMod, "KEY MOD");
	addSpacer();
	addKnob((VstInt32)Washington::ParamIndices::Gain, "GAIN");

	VstEditor::Open();
}
