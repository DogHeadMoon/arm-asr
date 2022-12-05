#include "avatr.h"

#include <iomanip>
#include <thread>
#include <random>


#include "decoder/params.h"
#include "frontend/wav.h"
#include "utils/flags.h"
#include "utils/string.h"
#include "utils/thread_pool.h"
#include "utils/timer.h"
#include "utils/utils.h"

#include "decoder/onnx_asr_model.h"


Engine::Engine(string model_dir){
    Init(model_dir);
}

void Engine::Init(string model_dir){
  //g_decode_config = wenet::InitDecodeOptionsFromFlags();
  //g_feature_config = wenet::InitFeaturePipelineConfigFromFlags();

  p_feature_config = std::make_shared<wenet::FeaturePipelineConfig>(80, 16000);
  p_feature_pipeline = std::make_shared<wenet::FeaturePipeline>(*p_feature_config);

  p_decode_resource = std::make_shared<wenet::DecodeResource>();
  const int num_threads = 1;
  wenet::OnnxAsrModel::InitEngineThreads(num_threads);
  auto model = std::make_shared<wenet::OnnxAsrModel>();
  model->Read(model_dir);
  p_decode_resource->model = model;

  string unit_path = model_dir + "/units.txt";

  p_decode_resource->symbol_table = std::shared_ptr<fst::SymbolTable>(
      fst::SymbolTable::ReadText(unit_path));
  p_decode_resource->unit_table = p_decode_resource->symbol_table;
  
  CHECK(p_decode_resource->unit_table != nullptr);
  LOG(INFO)<<"units path : "<<unit_path;

  wenet::PostProcessOptions post_process_opts;
  p_decode_resource->post_processor =
    std::make_shared<wenet::PostProcessor>(post_process_opts);

  p_decode_config = std::make_shared<wenet::DecodeOptions>();
  //p_decode_config->chunk_size = 16;
  p_decode_config->chunk_size = -1;

  p_decoder = std::make_shared<wenet::AsrDecoder>(p_feature_pipeline
    , p_decode_resource, *p_decode_config);
}

void Engine::write_audio(vector<int16_t> data, int status){
  if(data.size()>0){
    p_feature_pipeline->AcceptWaveform(data.data(), data.size());
  }
    
    if(status == 2){
      LOG(INFO)<<"write audio, set input finished "<<std::endl;
      if(p_feature_pipeline->input_finished() == false){
        p_feature_pipeline->set_input_finished(); 
      }
      else{
        LOG(INFO)<<"write audio, set input finished invalid, already finished"<<std::endl;
      }
    }
}

void Engine::reset(){
  p_decoder->Reset();
}

void Engine::reset_continuous_decoding(){
  if (p_decoder->DecodedSomething()) {
        p_decoder->Rescoring();
        LOG(INFO) << "Final result (continuous decoding): "
                  << p_decoder->result()[0].sentence;
  }

  p_decoder->ResetContinuousDecoding();
}

void Engine::start_decode(){
  t_.reset(new std::thread(&Engine::decode_thread_func, this));
  LOG(INFO) << "Engine::start_decode";
  t_->detach();
}

void Engine::decode_thread_func(){
  //std::thread::id id = std::this_thread::get_id();
  unsigned long long rid = std::hash<std::thread::id>{}(std::this_thread::get_id()) + int(rand());

  LOG(INFO)<<"tid : "<<rid<<"\t thead start" <<std::endl;
  while (true) {
    wenet::DecodeState state = p_decoder->Decode();
    LOG(INFO)<<"tid : "<<rid<<"\tI am in decode thread, decode state : "<<state <<std::endl;
    if (state == wenet::DecodeState::kEndFeats) {
      LOG(INFO)<<"tid : "<<rid<<"\tI am in thread rescore"<<std::endl;
      p_decoder->Rescoring();
      LOG(INFO)<<"tid : "<<rid<<"\tI am in thread rescore done"<<std::endl;
      //p_decoder->ResetContinuousDecoding();
    }

    /* 
    if (state == wenet::DecodeState::kEndFeats || state == wenet::DecodeState::kEndpoint) {
      std::cout<<"I am in thread rescore"<<std::endl;
      p_decoder->Rescoring();
    } */

    std::string result;
    if (p_decoder->DecodedSomething()) {
      result = p_decoder->result()[0].sentence;
    }
    result_ = result;

    if (state == wenet::DecodeState::kEndFeats) {
      LOG(INFO) << "wenet endfeats final result: " << result;
      if(result != "" && result !="嗯"){
          total_result_ += result + "。" + "\n";
      }

      LOG(INFO)<<"du log total result : "<<total_result_<<std::endl;

      //add by du
      //break;
      p_decoder->Reset();
      //p_decoder->ResetContinuousDecoding();
    } else if (state == wenet::DecodeState::kEndpoint) {
      LOG(INFO) << "wenet endpoint final result: " << result;
      total_result_ += result + "，";
      LOG(INFO) << "wenet endpoint current total final result: " << result;

      p_decoder->ResetContinuousDecoding();
    } else {
      if (p_decoder->DecodedSomething()) {
        LOG(INFO) << "wenet partial result: " << result;
      }
    }
  }
  LOG(INFO)<<"tid : "<<rid<<"\t thead die" <<std::endl;
}

string Engine::get_result() {
  return result_;
  /*
  std::string result;
  if (p_decoder->DecodedSomething()) {
      result = p_decoder->result()[0].sentence;
  }
  return result;*/
}

string Engine::get_total_result() {
  return total_result_;
}



/*
void Engine::start_decode() {
  std::thread decode_thread(decode_thread_func);
  decode_thread.detach();
}
*/