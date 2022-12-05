#ifndef AVATR_H_
#define AVATR_H_

#include <utility>

#include "decoder/asr_decoder.h"

using namespace std;

class Engine {
public:
    Engine(){};
    ~Engine(){};
    Engine(string model_dir);
    void Init(string model_dir);
    void write_audio(vector<int16_t> data, int status);
    void decode_thread_func();
    void start_decode();
    void reset();
    void reset_continuous_decoding();
    string get_result();
    string get_total_result();
    void stop();
    //void start_decode();

private:
  std::shared_ptr<wenet::DecodeOptions> p_decode_config;
  std::shared_ptr<wenet::FeaturePipelineConfig> p_feature_config;
  std::shared_ptr<wenet::FeaturePipeline> p_feature_pipeline;
  std::shared_ptr<wenet::DecodeResource> p_decode_resource;

  std::shared_ptr<wenet::AsrDecoder> p_decoder;
  std::string total_result_;
  std::string result_;
  std::unique_ptr<std::thread> t_{nullptr};
  int status_=0;
};


#endif