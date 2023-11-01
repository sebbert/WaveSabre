add_executable(WaveSabreEmscripten
	source/Main.js
	source/Main.cpp
	source/SongRenderer.cpp
	source/SongRenderer.Track.cpp

	include/WaveSabreEmscripten/Song.h
	include/WaveSabreEmscripten/SongRenderer.h
)

set_source_files_properties(source/Main.js PROPERTIES HEADER_FILE_ONLY TRUE)

add_custom_target(MainJs DEPENDS source/Main.js)
add_dependencies(WaveSabreEmscripten MainJs)

target_include_directories(WaveSabreEmscripten PUBLIC include)

target_link_libraries(WaveSabreEmscripten
	WaveSabreCore)

target_link_options(WaveSabreEmscripten PRIVATE
	"--post-js" "${CMAKE_CURRENT_SOURCE_DIR}/source/Main.js"
	"-gsource-map"
	"-sWASM=1"
	"-sEXPORTED_FUNCTIONS=_main,_renderSong"
	"-sEXPORTED_RUNTIME_METHODS=ccall,cwrap"
)
set_target_properties(WaveSabreEmscripten PROPERTIES SUFFIX ".html")

