#include <emscripten.h>

#include <string.h>

#include <WaveSabreEmscripten/SongRenderer.h>

using namespace WaveSabreCore;
using namespace WaveSabrePlayerLib;
#include <WaveSabreEmscripten/Song.h>

SongRenderer songRenderer(&Song, 1);

extern "C"
{
	EMSCRIPTEN_KEEPALIVE void renderSong(void* buffer, int sampleCount) {
		auto tmpBuffer = new SongRenderer::Sample[sampleCount];
		songRenderer.RenderSamples(tmpBuffer, sampleCount);
		memcpy(buffer, tmpBuffer, sampleCount * sizeof(*tmpBuffer));
		delete[] tmpBuffer;
	}

	EMSCRIPTEN_KEEPALIVE int main() {
		EM_ASM( playSong() );
		return 0;
	}
}