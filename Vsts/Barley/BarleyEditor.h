#pragma once

#include <WaveSabreCore.h>
#include <WaveSabreVstLib.h>

using namespace WaveSabreVstLib;

class BarleyEditor : public VstEditor
{
public:
	BarleyEditor(AudioEffect *audioEffect, WaveSabreCore::Barley *barley);
	virtual ~BarleyEditor();

	virtual void doIdleStuff ();

	virtual void Open();

private:
	CTextLabel* numGrainsLabel;
	WaveSabreCore::Barley* barley;
};
