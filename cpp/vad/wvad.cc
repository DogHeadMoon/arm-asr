#include "wvad.h"

#include <string>
#include <mutex>
//#include "onnxruntime_cxx_api.h"
#include <fstream>
#include <iostream>
#include <climits>
#include <ctime>
#include "webrtcvad.h"
using namespace std;

Wvad::Wvad(){
  pvad_ = std::make_shared<avatr::WebrtcVad>(mode, sample_rate_);
}

bool Wvad::process(vector<int16_t> chunk) {
  bool speaking = pvad_->Process(chunk.data(), chunk.size());

  if (speaking) {
    consecutiveSilence_ = 0;
    consecutiveSpeaking_++;
    vad_status_ = 2;
  } else {
    consecutiveSilence_++;
    consecutiveSpeaking_ = 0;
    vad_status_ = 0;
  }

  if (!speaking_ &&
      consecutiveSpeaking_ == consecutiveFramesForSpeaking) {
    speaking_ = true;
    vad_status_ = 1;
  }

  if (speaking_ &&
      consecutiveSilence_ == consecutiveFramesForSilence) {
    speaking_ = false;
    vad_status_ = 3;
  }

  if(speaking_ && vad_status_ != 1){
    vad_status_ = 2;
  }

  if(!speaking_ && vad_status_ != 3){
    vad_status_ = 0;
  }
  //std::cout <<frame_idx_ ++ <<"\t" <<speaking << "\t" <<speaking_ << "\t"<< status()<<std::endl;
  return speaking_;
}

int Wvad::status(){
  return vad_status_;
}
