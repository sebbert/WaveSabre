#include "VectronEditor.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

VectronEditor::VectronEditor(AudioEffect *audioEffect)
	: VstEditor(audioEffect, 600, 700, "VECTRON")
{
}

VectronEditor::~VectronEditor()
{
}

#define MOD_KNOBS(PARAM, NAME) \
	do { \
		int prevX = currentX; \
		int prevY = currentY; \
		addKnob((VstInt32)PARAM, NAME); \
		currentX = prevX; currentY += RowHeight; \
		addKnob((VstInt32)PARAM##Env1Amt, "ENV1"); \
		currentX = prevX; currentY += RowHeight; \
		addKnob((VstInt32)PARAM##Env2Amt, "ENV2"); \
		currentY = prevY; \
	} while(0)


void VectronEditor::Open()
{
	MOD_KNOBS(Vectron::ParamIndices::ModScale, "MOD");
	addSpacer();
	MOD_KNOBS(Vectron::ParamIndices::ModOffset, "OFFSET");

	addSpacer();
	MOD_KNOBS(Vectron::ParamIndices::ModXScale, "X MOD");
	MOD_KNOBS(Vectron::ParamIndices::ModXOffset, "X OFFSET");
	MOD_KNOBS(Vectron::ParamIndices::ModXDetune, "X DETUNE");

	addSpacer();
	MOD_KNOBS(Vectron::ParamIndices::ModYScale, "Y MOD");
	MOD_KNOBS(Vectron::ParamIndices::ModYOffset, "Y OFFSET");
	MOD_KNOBS(Vectron::ParamIndices::ModYDetune, "Y DETUNE");

	startNextRow();
	startNextRow();
	startNextRow();
	startNextRow();

	MOD_KNOBS(Vectron::ParamIndices::Osc1Offset, "OFFSET");
	addSpacer();
	MOD_KNOBS(Vectron::ParamIndices::Osc1Mod, "MOD");
	addSpacer();
	addSpacer();

	startNextRow();
	startNextRow();
	startNextRow();
	startNextRow();

	addKnob((VstInt32)Vectron::ParamIndices::Env1Attack,  "ENV1 A");
	addKnob((VstInt32)Vectron::ParamIndices::Env1Decay,   "ENV1 D");
	addKnob((VstInt32)Vectron::ParamIndices::Env1Sustain, "ENV1 S");
	addKnob((VstInt32)Vectron::ParamIndices::Env1Release, "ENV1 R");

	startNextRow();

	addKnob((VstInt32)Vectron::ParamIndices::Env2Attack,  "ENV2 A");
	addKnob((VstInt32)Vectron::ParamIndices::Env2Decay,   "ENV2 D");
	addKnob((VstInt32)Vectron::ParamIndices::Env2Sustain, "ENV2 S");
	addKnob((VstInt32)Vectron::ParamIndices::Env2Release, "ENV2 R");


	VstEditor::Open();
}
