#ifndef __VECTRONEDITOR_H__
#define __VECTRONEDITOR_H__

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class VectronEditor : public VstEditor
{
public:
	VectronEditor(AudioEffect *audioEffect);
	virtual ~VectronEditor();

	virtual void Open();
};

#endif
