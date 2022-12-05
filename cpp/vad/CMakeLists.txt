#project(demo)
#include_directories("/home/mathew/workplace/wenet-latest/wenet/runtime/onnxruntime/fc_base/onnxruntime-src/include")
#link_directories("/home/mathew/workplace/wenet-latest/wenet/runtime/onnxruntime/fc_base/onnxruntime-src/lib")

#add_executable(demo vad.cc)
#target_link_libraries(demo onnxruntime)
include_directories(..)
file(GLOB_RECURSE SOURCES
        ../webrtc/*.c
        ../webrtc/*.cc
)

add_compile_options(
        -DWEBRTC_POSIX
)

add_library(wvad STATIC wvad.cc webrtcvad.cc ${SOURCES})
