#pragma once

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class WashingtonEditor : public VstEditor
{
public:
	WashingtonEditor(AudioEffect *audioEffect);
	virtual ~WashingtonEditor();

	virtual void Open();
};
