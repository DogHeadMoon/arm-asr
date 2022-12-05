cmake_minimum_required(VERSION 3.4.1)
set(TARGET wenet)
#add by du
#project(${TARGET} CXX)
project(${TARGET} CXX C)

set(CMAKE_CXX_STANDARD 14)
include(ExternalProject)

option(TORCH "whether to build with Torch" OFF)
option(ONNX "whether to build with ONNX" ON)
set(CMAKE_VERBOSE_MAKEFILE on)
set(build_DIR ${CMAKE_SOURCE_DIR}/../../../build)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
string(REPLACE "-Wl,--exclude-libs,libgcc_real.a" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

#add by du
include(onnx)
#include(libtorch)
include(openfst)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/kaldi
  C:/workplace/githubs/wenet/runtime/android/app/build/pytorch_android-1.10.0.aar/headers
)

#add by du
#include_directories(${build_DIR}/onnxruntime-1.11.1-android/onnx/include)
#link_directories(${build_DIR}/onnxruntime-1.11.1-android/lib)
#ONNX
set(ONNX_DIR "${CMAKE_SOURCE_DIR}/../onnx")
find_package(ONNX REQUIRED)
if (ONNX_FOUND)
    message(STATUS "ONNX_LIBS: ${ONNX_LIBS}")
    message(STATUS "ONNX_INCLUDE_DIRS: ${ONNX_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "onnxruntime Not Found!")
endif (ONNX_FOUND)

#[[
include_directories(.)
set(SOURCES
        webrtc/common_audio/signal_processing/complex_bit_reverse.c
        webrtc/common_audio/signal_processing/complex_fft.c
        webrtc/common_audio/signal_processing/cross_correlation.c
        webrtc/common_audio/signal_processing/division_operations.c
        webrtc/common_audio/signal_processing/downsample_fast.c
        webrtc/common_audio/signal_processing/energy.c
        webrtc/common_audio/signal_processing/get_scaling_square.c
        webrtc/common_audio/signal_processing/min_max_operations.c
        webrtc/common_audio/signal_processing/resample_48khz.c
        webrtc/common_audio/signal_processing/resample_by_2_internal.c
        webrtc/common_audio/signal_processing/resample_fractional.c
        webrtc/common_audio/signal_processing/spl_init.c
        webrtc/common_audio/signal_processing/spl_inl.c
        webrtc/common_audio/signal_processing/spl_sqrt.c
        webrtc/common_audio/signal_processing/vector_scaling_operations.c
        webrtc/common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.c
        webrtc/common_audio/vad/vad_core.c
        webrtc/common_audio/vad/vad_filterbank.c
        webrtc/common_audio/vad/vad_gmm.c
        webrtc/common_audio/vad/vad_sp.c
        webrtc/common_audio/vad/webrtc_vad.c

        webrtc/rtc_base/checks.cc
        )

add_compile_options(
        -DWEBRTC_POSIX
)
]]

add_subdirectory(utils)
add_subdirectory(frontend)
add_subdirectory(post_processor)
add_subdirectory(kaldi)  # kaldi: wfst based decoder
add_subdirectory(decoder)
add_subdirectory(avatr)
add_subdirectory(vad)

#add by du
link_libraries(frontend avatr wvad decoder android)
#add_library(${TARGET} SHARED wenet.cc vad/wvad.cc vad/webrtcvad.cc ${SOURCES})
add_library(${TARGET} SHARED wenet.cc)

add_executable(decoder_main bin/decoder_main.cc)
target_link_libraries(decoder_main PUBLIC libc++_shared.so)
