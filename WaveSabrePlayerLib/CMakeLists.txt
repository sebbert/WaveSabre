add_library(WaveSabrePlayerLib
	include/WaveSabrePlayerLib/CriticalSection.h
	include/WaveSabrePlayerLib/PreRenderPlayer.h
	include/WaveSabrePlayerLib/WavWriter.h
	include/WaveSabrePlayerLib/DirectSoundRenderThread.h
	include/WaveSabrePlayerLib/RealtimePlayer.h
	include/WaveSabrePlayerLib/IPlayer.h
	include/WaveSabrePlayerLib/SongRenderer.h
	src/CriticalSection.cpp
	src/DirectSoundRenderThread.cpp
	src/IPlayer.cpp
	src/PreRenderPlayer.cpp
	src/RealtimePlayer.cpp
	src/SongRenderer.cpp
	src/SongRenderer.Track.cpp
	src/WavWriter.cpp)

target_link_libraries(WaveSabrePlayerLib
	WaveSabreCore)

target_include_directories(WaveSabrePlayerLib PUBLIC include)

