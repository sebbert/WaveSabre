#include "BarleyEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

BarleyEditor::BarleyEditor(AudioEffect *audioEffect)
	: VstEditor(audioEffect, 250, 300, "Barley")
{
}

BarleyEditor::~BarleyEditor()
{
}

void BarleyEditor::Open()
{
	addKnob((VstInt32)Barley::ParamIndices::Position, "POSITION");
	addSpacer();
	addKnob((VstInt32)Barley::ParamIndices::Size, "SIZE");
	startNextRow();
	
	
	
	addKnob((VstInt32)Barley::ParamIndices::DryWet, "DRY/WET");

	VstEditor::Open();
}
