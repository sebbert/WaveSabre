#ifndef __STRIKERVST_H__
#define __STRIKERVST_H__

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class StrikerVst : public VstPlug
{
public:
	StrikerVst(audioMasterCallback audioMaster);

	virtual void getParameterName(VstInt32 index, char *text);

	virtual bool getEffectName(char *name);
	virtual bool getProductString(char *text);
};

#endif
