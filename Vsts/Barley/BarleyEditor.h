#pragma once

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class BarleyEditor : public VstEditor
{
public:
	BarleyEditor(AudioEffect *audioEffect);
	virtual ~BarleyEditor();

	virtual void Open();
};
