#pragma once

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class BarleyVst : public VstPlug
{
public:
	BarleyVst(audioMasterCallback audioMaster);

	virtual void getParameterName(VstInt32 index, char *text);
	virtual bool canParameterBeAutomated(VstInt32 index);

	virtual bool getEffectName(char *name);
	virtual bool getProductString(char *text);
};
