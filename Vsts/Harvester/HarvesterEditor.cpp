#include "HarvesterEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

HarvesterEditor::HarvesterEditor(AudioEffect *audioEffect)
	: VstEditor(audioEffect, 250, 300, "HARVESTER")
{
}

HarvesterEditor::~HarvesterEditor()
{
}

void HarvesterEditor::Open()
{
	addKnob((VstInt32)Harvester::ParamIndices::Position, "POSITION");
	addSpacer();
	addKnob((VstInt32)Harvester::ParamIndices::Size, "SIZE");
	startNextRow();
	
	addKnob((VstInt32)Harvester::ParamIndices::StereoSpread, "STEREO");
	addSpacer();
	addKnob((VstInt32)Harvester::ParamIndices::Reverb, "REVERB");
	startNextRow();
	
	
	addKnob((VstInt32)Harvester::ParamIndices::DryWet, "DRY/WET");

	VstEditor::Open();
}
