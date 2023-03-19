#ifndef __WAVESABREVSTLIB_VSTEDITOR_H__
#define __WAVESABREVSTLIB_VSTEDITOR_H__

#include "Common.h"
#include "ImageManager.h"
#include "NoTextCOptionMenu.h"

namespace WaveSabreVstLib
{
	class VstEditor : public AEffGUIEditor, public CControlListener
	{
	public:
		VstEditor(AudioEffect *audioEffect, int width, int height, std::string title);
		virtual ~VstEditor();

		virtual void Open();
		virtual void Close();

		virtual void setParameter(VstInt32 index, float value);
		virtual void valueChanged(CControl *control);

		virtual bool open(void *ptr);
		virtual void close();

		const int SpacerWidth = BaseSize;

	protected:
		static const int BaseSize = 20;
		static const int LeftMargin = BaseSize;

		static const int TitleTopMargin = BaseSize / 2;
		static const int TitleAreaHeight = BaseSize * 2;

		static const int RowHeight = BaseSize * 3;

		static const int KnobWidth = 55;
		static const int KnobKnobOffset = 12;
		static const int KnobCaptionWidth = 100;
		static const int KnobCaptionOffset = 30;

		static const int ButtonWidth = 55;
		static const int ButtonButtonOffset = 18;
		static const int ButtonCaptionWidth = KnobCaptionWidth;
		static const int ButtonCaptionOffset = 14;

		static const int OptionMenuWidth = 100;
		static const int OptionMenuButtonOffset = 0;
		static const int OptionMenuCaptionWidth = 120;
		static const int OptionMenuCaptionOffset = 18;


		void startNextRow();

		CTextLabel *addTextLabel(int x, int y, int w, int h, std::string text, CFontRef fontId = kNormalFontVeryBig, CHoriTxtAlign textAlign = kLeftText);

		CAnimKnob *addKnob(VstInt32 param, std::string caption);
		CKickButton *addButton(VstInt32 param, std::string caption);
		NoTextCOptionMenu *addOptionMenu(std::string caption);

		void addSpacer();

		int currentX, currentY;

	private:
		std::string title;

		std::map<VstInt32, CControl *> controls;
	};
}

#endif
