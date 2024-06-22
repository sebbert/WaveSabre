project("WaveSabre" LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "-Os -msimd128 -fno-rtti -fno-exceptions")
endif()

# shared code
add_subdirectory(WaveSabreCore)
add_subdirectory(WaveSabreEmscripten)
# Needs to be ported properly add_subdirectory(WaveSabrePlayerLib)
