set(decoder_srcs
  avatr.cc
#  ../decoder/asr_decoder.cc
        #  ../decoder/asr_model.cc
        # ../decoder/context_graph.cc
        # ../decoder/ctc_prefix_beam_search.cc
        # ../decoder/ctc_wfst_beam_search.cc
        #../decoder/ctc_endpoint.cc
)

add_library(avatr STATIC avatr.cc)
target_link_libraries(avatr PUBLIC decoder)

#list(APPEND decoder_srcs ../decoder/onnx_asr_model.cc)

#add_library(avatr STATIC ${decoder_srcs})
