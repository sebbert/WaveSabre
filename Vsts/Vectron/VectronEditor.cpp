#include "VectronEditor.h"

#include "PhaseModPlotView.h"
#include "OscillatorPlotView.h"

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

VectronEditor::VectronEditor(VectronVst *vst)
	: VstEditor(vst, 800, 700, "VECTRON")
	, vectron(vst->Device)
	, phaseModPlot(nullptr)
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
	MOD_KNOBS(Vectron::ParamIndices::ModOffset, "OFFSET");
	addSpacer();

	MOD_KNOBS(Vectron::ParamIndices::ModXScale, "X MOD");
	MOD_KNOBS(Vectron::ParamIndices::ModXOffset, "X OFFSET");
	MOD_KNOBS(Vectron::ParamIndices::ModXDetune, "X DETUNE");
	addSpacer();

	MOD_KNOBS(Vectron::ParamIndices::ModYScale, "Y MOD");
	MOD_KNOBS(Vectron::ParamIndices::ModYOffset, "Y OFFSET");
	MOD_KNOBS(Vectron::ParamIndices::ModYDetune, "Y DETUNE");
	addSpacer();
	
	if (phaseModPlot == nullptr)
	{
		constexpr auto size = RowHeight * 3;

		CRect plotRect(currentX, currentY, currentX + size, currentY + size);

		phaseModPlot = new PhaseModPlotView(vectron, plotRect);
		phaseModPlot->invalid();
		frame->addView(phaseModPlot);
	}

	startNextRow();
	startNextRow();
	startNextRow();
	startNextRow();

	if (osc1Plot == nullptr)
	{
		constexpr auto size = RowHeight * 3;

		CRect plotRect(currentX, currentY, currentX + KnobWidth*2, currentY + RowHeight);

		osc1Plot = new OscillatorPlotView(vectron, plotRect);
		osc1Plot->invalid();
		frame->addView(osc1Plot);
	}

	MOD_KNOBS(Vectron::ParamIndices::Osc1Offset, "OSC1 OFFSET");
	addSpacer();
	MOD_KNOBS(Vectron::ParamIndices::Osc1Mod, "OSC1 MOD");

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

void VectronEditor::Close()
{
	if (phaseModPlot)
	{
		frame->removeView(phaseModPlot, false);
	}

	if (osc1Plot)
	{
		frame->removeView(osc1Plot);
	}
}

void VectronEditor::setParameter(VstInt32 index, float value)
{
	frame->invalid();

	VstEditor::setParameter(index, value);
}

void VectronEditor::valueChanged(CControl *control)
{
	frame->invalid();

	VstEditor::valueChanged(control);
}
