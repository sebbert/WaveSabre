#ifndef __HARVESTERVST_H__
#define __HARVESTERVST_H__

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class HarvesterVst : public VstPlug
{
public:
	HarvesterVst(audioMasterCallback audioMaster);

	virtual void getParameterName(VstInt32 index, char *text);

	virtual bool getEffectName(char *name);
	virtual bool getProductString(char *text);
};

#endif