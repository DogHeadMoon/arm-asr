// Copyright (c) 2021 Mobvoi Inc (authors: Xiaoyu Chen)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <jni.h>
#include <thread>
//#include <vector>
//#include <string>
#include "avatr/avatr.h"
#include "utils/string.h"
#include "vad/wvad.h"

namespace wenet {
/*
std::shared_ptr<DecodeOptions> decode_config;
std::shared_ptr<FeaturePipelineConfig> feature_config;
std::shared_ptr<FeaturePipeline> feature_pipeline;
std::shared_ptr<AsrDecoder> decoder;
std::shared_ptr<DecodeResource> resource;
*/
//DecodeState state = kEndBatch;
    std::string total_result;  // NOLINT
    std::shared_ptr<Engine> pEng = std::make_shared<Engine>();
    //std::shared_ptr<Engine> pEng;

    bool use_vad = true;
    bool stopped_ = false;
    bool has_reset = false;
    Wvad vad;

    const float interval = 0.03;
    const int sample_rate = 16000;
    const int sample_interval = interval * sample_rate;

    int state = 0;


    void init(JNIEnv* env, jobject, jstring jModelDir) {

      const char* pModelDir = env->GetStringUTFChars(jModelDir, nullptr);
      pEng->Init(pModelDir);
    }

    void reset(JNIEnv *env, jobject) {
      LOG(INFO) << "wenet reset";
      //decoder->Reset();
      //state = kEndBatch;
      pEng->reset();
      state = 0;
      total_result = "";
    }

    void accept_waveform(JNIEnv *env, jobject, jshortArray jWaveform) {
      jsize size = env->GetArrayLength(jWaveform);
      int16_t* waveform = env->GetShortArrayElements(jWaveform, 0);
      //feature_pipeline->AcceptWaveform(waveform, size);
      LOG(INFO) << "wenet accept_waveform "<<size<<" samples waveform";
      //vector<int16_t> chunk(size);

      for (int start = 0; start < size; start += sample_interval) {
        int end = std::min(start + sample_interval, size);

        // Convert to short
        std::vector<int16_t> data;
        data.reserve(end - start);
        for (int j = start; j < end; j++) {
          data.emplace_back(waveform[j]);
        }

        bool bvad = vad.process(data);

        LOG(INFO)<<"vad : " <<bvad << "  vad status : "<<vad.status();

        if (bvad == false && vad.status() != 3){
          continue;
        }

        int status = vad.status();
        if(status == 1){
          pEng->write_audio(std::move(data), 0);
        }
        else if(status == 3){
          pEng->write_audio(std::move(data), 2);
        }
        else{
          pEng->write_audio(std::move(data), 1);
        }

      }
    }

    void set_input_finished() {
      LOG(INFO) << "wenet input finished";
      std::vector<int16_t> chunk;
      pEng->write_audio(std::move(chunk), 2);
      state=5;
    }

    void start_decode() {
      pEng->start_decode();
    }

    jboolean get_finished(JNIEnv *env, jobject) {
      if (state == 5) {
        LOG(INFO) << "wenet recognize finished";
        return JNI_TRUE;
      }
      return JNI_FALSE;
    }

    jstring get_result(JNIEnv *env, jobject) {
      return env->NewStringUTF(pEng->get_total_result().c_str());
    }
}  // namespace wenet

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
  JNIEnv *env;
  if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  }

  jclass c = env->FindClass("com/mobvoi/wenet/Recognize");
  if (c == nullptr) {
    return JNI_ERR;
  }

  static const JNINativeMethod methods[] = {
          {"init", "(Ljava/lang/String;)V", reinterpret_cast<void*>(wenet::init)},
          {"reset", "()V", reinterpret_cast<void *>(wenet::reset)},
          {"acceptWaveform", "([S)V",
                                            reinterpret_cast<void *>(wenet::accept_waveform)},
          {"setInputFinished", "()V",
                                            reinterpret_cast<void *>(wenet::set_input_finished)},
          {"getFinished", "()Z", reinterpret_cast<void *>(wenet::get_finished)},
          {"startDecode", "()V", reinterpret_cast<void *>(wenet::start_decode)},
          {"getResult", "()Ljava/lang/String;",
                                            reinterpret_cast<void *>(wenet::get_result)},
  };
  int rc = env->RegisterNatives(c, methods,
                                sizeof(methods) / sizeof(JNINativeMethod));

  if (rc != JNI_OK) {
    return rc;
  }

  return JNI_VERSION_1_6;
}
