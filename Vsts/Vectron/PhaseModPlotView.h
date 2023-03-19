#ifndef __PHASEMODPLOTVIEW_H__
#define __PHASEMODPLOTVIEW_H__

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;


class PhaseModPlotView : public CView
{
public:
	PhaseModPlotView (Vectron *vectron, const CRect &size)
		: CView(size)
		, vectron(vectron)
	{
	}

	virtual ~PhaseModPlotView()
	{
	}

	virtual void draw (CDrawContext *ctx)
	{
		constexpr CCoord margin = 2;
		constexpr auto numSamples = 64;

		ctx->setDrawMode(kCopyMode);
		
		ctx->setFillColor(kTransparentCColor);
		ctx->drawRect(size, kDrawFilled);

		ctx->setDrawMode(kAntialias);
		ctx->setLineStyle(kLineSolid);
		ctx->setLineWidth(1.5);
		ctx->setFrameColor(MakeCColor(77, 19, 210, 255));

		const Vectron::PhaseModulator *phaseMod = &vectron->phaseMod;
		Vectron::PhaseModulator::State phaseModState;

		Vectron::Modulation mod;
		mod.Env1 = 0.0f;
		mod.Env2 = 0.0f;

		for (int i = 0; i < numSamples; ++i)
		{
			double phase = (double)i / (numSamples-1);
			phaseModState.PhaseX = phase;
			phaseModState.PhaseY = phase;

			phaseMod->Next(&phaseModState, 69.0, &mod);

			auto width = (size.width() - 2*margin);
			auto height = (size.height() - 2*margin);

			CPoint point(
				phaseModState.OutputX * width + size.left + margin,
				phaseModState.OutputY * height + size.top + margin
			);

			if (i == 0)
				ctx->moveTo(point);
			else
				ctx->lineTo(point);
		}

		setDirty(false);
	}
protected:
	Vectron *vectron;
};

#endif
