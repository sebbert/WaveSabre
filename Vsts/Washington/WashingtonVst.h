#pragma once

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class WashingtonVst : public VstPlug
{
public:
	WashingtonVst(audioMasterCallback audioMaster);

	virtual void getParameterName(VstInt32 index, char *text);

	virtual bool getEffectName(char *name);
	virtual bool getProductString(char *text);
};
