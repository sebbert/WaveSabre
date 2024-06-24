#include <emscripten.h>

#include <string.h>

#include <WaveSabreCore/EmscriptenSample.h>
#include <WaveSabreEmscripten/SongRenderer.h>

using namespace WaveSabreCore;
using namespace WaveSabrePlayerLib;

#include <WaveSabreEmscripten/Song.h>

using Sample = SongRenderer::Sample;

extern "C"
{
	EMSCRIPTEN_KEEPALIVE
	Sample* WaveSabre_CreateBuffer(size_t len) {
		return new Sample[len];
	}

	EMSCRIPTEN_KEEPALIVE
	void WaveSabre_DestroyBuffer(Sample* buffer) {
		delete[] buffer;
	}

	EMSCRIPTEN_KEEPALIVE
	SongRenderer* WaveSabre_CreateSongRenderer() {
		return new SongRenderer(&Song, 1);
	}

	EMSCRIPTEN_KEEPALIVE
	void WaveSabre_DestroySongRenderer(SongRenderer* renderer) {
		delete renderer;
	}

	EMSCRIPTEN_KEEPALIVE
	void WaveSabre_RenderSamples(SongRenderer* renderer, Sample* buffer, size_t len) {
		renderer->RenderSamples(buffer, len);
	}
}
