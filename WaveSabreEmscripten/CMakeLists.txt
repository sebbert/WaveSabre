add_executable(WaveSabreEmscripten
	source/Main.cpp
	source/SongRenderer.cpp
	source/SongRenderer.Track.cpp

	include/WaveSabreEmscripten/Song.h
	include/WaveSabreEmscripten/SongRenderer.h
)

target_include_directories(WaveSabreEmscripten PUBLIC include)

target_link_libraries(WaveSabreEmscripten
	WaveSabreCore)

set(EMSCRIPTEN_EXPORTED_FUNCTIONS
	"_main"
	"_WaveSabre_CreateBuffer"
	"_WaveSabre_DestroyBuffer"
	"_WaveSabre_CreateSongRenderer"
	"_WaveSabre_DestroySongRenderer"
	"_WaveSabre_RenderSamples"
)

string(JOIN "," EMSCRIPTEN_EXPORTED_FUNCTIONS_ARG ${EMSCRIPTEN_EXPORTED_FUNCTIONS})

target_link_options(WaveSabreEmscripten PRIVATE
	"-gsource-map"
	"-sWASM=1"
	"-sEXPORTED_FUNCTIONS=${EMSCRIPTEN_EXPORTED_FUNCTIONS_ARG}"
	"-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,addOnPostRun"
)
set_target_properties(WaveSabreEmscripten PROPERTIES SUFFIX ".js")

