#include "BarleyEditor.h"
#include "BarleyVst.h"
#include <WaveSabreCore.h>

#include <string>
#include <sstream>

using namespace WaveSabreCore;

BarleyEditor::BarleyEditor(AudioEffect *audioEffect, Barley* barley)
	: VstEditor(audioEffect, 250, 300, "Barley")
	, barley(barley)
{
}

BarleyEditor::~BarleyEditor()
{
}

void BarleyEditor::Open()
{
	addKnob((VstInt32)Barley::ParamIndices::GrainPosition, "POSITION");
	addSpacer();
	addKnob((VstInt32)Barley::ParamIndices::GrainSize, "SIZE");
	startNextRow();

	VstEditor::Open();
}

void BarleyEditor::doIdleStuff()
{
}