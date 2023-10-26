project("WaveSabre" LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# shared code
add_subdirectory(WaveSabreCore)
# Needs to be ported properly add_subdirectory(WaveSabrePlayerLib)
