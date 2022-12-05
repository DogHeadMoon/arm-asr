#pragma once
#include "webrtcvad.h"
#include <vector>

using namespace std;

class Wvad {
public:
    Wvad();
    bool process(vector<int16_t> chunk);
    int status();

private:
    //avatr::WebrtcVad webrtc_vad_(1, 16000);
    
    std::shared_ptr<avatr::WebrtcVad> pvad_;

    int consecutiveSilence_ = 0;
  	int consecutiveSpeaking_ = 0;

	int consecutiveFramesForSilence = 5;
    //int consecutiveFramesForSilence = 10;
	int consecutiveFramesForSpeaking = 1;

    int64_t sample_rate_ = 16000;
    bool speaking_ = false;

    float sileroVadSilenceThreshold_ = 0.5;
    //float sileroVadSpeakingThreshold_ = 0.1;
    float sileroVadSpeakingThreshold_ = 0.5;
    int frame_idx_=0;
    int vad_status_ = 0;
    int mode = 1;
};

