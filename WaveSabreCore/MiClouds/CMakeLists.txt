
# paths to our sources
set(MUTABLE_PATH "${CMAKE_SOURCE_DIR}/eurorack")
set(STMLIB_PATH ${MUTABLE_PATH}/stmlib)
set(MI_CLOUDS_PATH ${MUTABLE_PATH}/clouds)

include_directories(${MUTABLE_PATH})

if(MSVC)
        add_definitions(-D_USE_MATH_DEFINES) # defines M_PI with MSVC
endif()

# Don't use ARM-specific implementations
add_compile_definitions(TEST)

set(STMLIB_SOURCES 
	${STMLIB_PATH}/stmlib.h
	${STMLIB_PATH}/utils/random.cc
	${STMLIB_PATH}/utils/random.h
	${STMLIB_PATH}/utils/dsp.h
	${STMLIB_PATH}/dsp/atan.cc
	${STMLIB_PATH}/dsp/atan.h
	${STMLIB_PATH}/dsp/units.cc
	${STMLIB_PATH}/dsp/units.h
	${STMLIB_PATH}/dsp/dsp.h
	${STMLIB_PATH}/fft/shy_fft.h
)

set(MI_SOURCES
	${MI_CLOUDS_PATH}/dsp/audio_buffer.h
	${MI_CLOUDS_PATH}/dsp/correlator.cc
	${MI_CLOUDS_PATH}/dsp/correlator.h
	${MI_CLOUDS_PATH}/dsp/frame.h
	${MI_CLOUDS_PATH}/dsp/fx/diffuser.h
	${MI_CLOUDS_PATH}/dsp/fx/fx_engine.h
	${MI_CLOUDS_PATH}/dsp/fx/pitch_shifter.h
	${MI_CLOUDS_PATH}/dsp/fx/reverb.h
	${MI_CLOUDS_PATH}/dsp/grain.h
	${MI_CLOUDS_PATH}/dsp/granular_processor.cc
	${MI_CLOUDS_PATH}/dsp/granular_processor.h
	${MI_CLOUDS_PATH}/dsp/granular_sample_player.h
	${MI_CLOUDS_PATH}/dsp/looping_sample_player.h
	${MI_CLOUDS_PATH}/dsp/mu_law.cc
	${MI_CLOUDS_PATH}/dsp/mu_law.h
	${MI_CLOUDS_PATH}/dsp/parameters.h

	${MI_CLOUDS_PATH}/dsp/pvoc/frame_transformation.cc
	${MI_CLOUDS_PATH}/dsp/pvoc/frame_transformation.h
	${MI_CLOUDS_PATH}/dsp/pvoc/phase_vocoder.cc
	${MI_CLOUDS_PATH}/dsp/pvoc/phase_vocoder.h
	${MI_CLOUDS_PATH}/dsp/pvoc/stft.cc
	${MI_CLOUDS_PATH}/dsp/pvoc/stft.h

	${MI_CLOUDS_PATH}/dsp/sample_rate_converter.h
	${MI_CLOUDS_PATH}/dsp/window.h
	${MI_CLOUDS_PATH}/dsp/wsola_sample_player.h

	${MI_CLOUDS_PATH}/resources.cc
	${MI_CLOUDS_PATH}/resources.h
)


add_library(
	MiClouds STATIC
    ${STMLIB_SOURCES}
    ${MI_SOURCES}
)
