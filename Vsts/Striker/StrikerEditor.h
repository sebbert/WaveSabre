#ifndef __STRIKEREDITOR_H__
#define __STRIKEREDITOR_H__

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class StrikerEditor : public VstEditor
{
public:
	StrikerEditor(AudioEffect *audioEffect);
	virtual ~StrikerEditor();

	virtual void Open();
};

#endif
