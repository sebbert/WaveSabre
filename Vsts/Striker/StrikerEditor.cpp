#include "StrikerEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

StrikerEditor::StrikerEditor(AudioEffect *audioEffect)
	: VstEditor(audioEffect, 820, 220, "STRIKER")
{
}

StrikerEditor::~StrikerEditor()
{
}

void StrikerEditor::Open()
{
	addKnob((VstInt32)Striker::ParamIndices::NoiseImpulseLevel, "NOISE IMPULSE");
	addSpacer();

	startNextRow();

	addKnob((VstInt32)Striker::ParamIndices::SineImpulseLevel, "SINE IMPULSE");
	addSpacer();

	VstEditor::Open();
}
