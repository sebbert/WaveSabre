#ifndef __VECTRONEDITOR_H__
#define __VECTRONEDITOR_H__

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;


#include "VectronVst.h"

class VectronEditor : public VstEditor
{
public:
	VectronEditor(VectronVst *vst);
	virtual ~VectronEditor();

	virtual void Open();
	virtual void Close();

	virtual void setParameter(VstInt32 index, float value);
	virtual void valueChanged(CControl *control);

protected:
	Vectron *vectron;

	class PhaseModPlotView *phaseModPlot;
};

#endif
