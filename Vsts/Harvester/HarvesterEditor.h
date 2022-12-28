#ifndef __HARVESTEREDITOR_H__
#define __HARVESTEREDITOR_H__

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class HarvesterEditor : public VstEditor
{
public:
	HarvesterEditor(AudioEffect *audioEffect);
	virtual ~HarvesterEditor();

	virtual void Open();
};

#endif
