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
	addKnob((VstInt32)Striker::ParamIndices::NoiseImpactLevel, "NOISE IMPULSE");
	addSpacer();

	startNextRow();

	addKnob((VstInt32)Striker::ParamIndices::SineImpactLevel, "SINE IMPULSE");
	addSpacer();

	VstEditor::Open();
}
