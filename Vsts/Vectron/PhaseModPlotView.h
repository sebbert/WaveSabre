#ifndef __PHASEMODPLOTVIEW_H__
#define __PHASEMODPLOTVIEW_H__

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

static float gradient[]
{
	0.3, 0.1, 0.95,
	0.7, 0.4, 0.1,
	0.95, 0.1, 0.4
};
static constexpr auto gradientSteps = 3;

static CColor sampleGradient(float phase)
{
	if (phase < 0.f) phase = 0.f;
	if (phase > 1.f) phase = 1.f;
	auto indexFloat = phase * (gradientSteps-1);
	auto indexFloor = floorf(indexFloat);
	auto p = indexFloat - indexFloor;
	auto c1 = &gradient[3*(int)indexFloor];
	auto c2 = &gradient[3*(int)indexFloor+3];
	return MakeCColor(
		(unsigned char)(255.0f * (c1[0] * (1.0f - p) + c2[0] * p)),
		(unsigned char)(255.0f * (c1[1] * (1.0f - p) + c2[1] * p)),
		(unsigned char)(255.0f * (c1[2] * (1.0f - p) + c2[2] * p)),
		255
	);
}

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
		constexpr auto numSamples = 128;
	
		auto width = (size.width() - 2*margin);
		auto height = (size.height() - 2*margin);

		ctx->setDrawMode(kCopyMode);
		
		ctx->setFillColor(kTransparentCColor);
		//ctx->drawRect(size, kDrawFilled);

		ctx->setDrawMode(kAntialias);
		ctx->setLineStyle(kLineSolid);
		// ctx->setLineStyle(kLineOnOffDash);
		ctx->setLineWidth(2);
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

			CPoint point(
				phaseModState.OutputX * width + size.left + margin,
				phaseModState.OutputY * height + size.top + margin
			);

			auto color = sampleGradient(phase);
			ctx->setFrameColor(color);
			
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
