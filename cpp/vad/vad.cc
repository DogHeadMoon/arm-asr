#include "vad.h"
using namespace std;

vector<int16_t> read(string path){
  wenet::WavReader wav_reader(path);
  const int sample_rate = 16000;
  // Only support 16K
  const int num_samples = wav_reader.num_samples();
  std::vector<int16_t> data;
  data.reserve(num_samples);
  for (int i= 0; i < num_samples; i++) {
  	data.push_back(static_cast<int16_t>(wav_reader.data()[i]));
  }
  return data;

  /*
  // Send data every 0.5 second
  const float interval = 0.1;
  const int sample_interval = interval * sample_rate;
  for (int start = 0; start < num_samples; start += sample_interval) {
    int end = std::min(start + sample_interval, num_samples);
    // Convert to short
    std::vector<int16_t> data;
    data.reserve(end - start);
    for (int j = start; j < end; j++) {
      data.push_back(static_cast<int16_t>(wav_reader.data()[j]));
	}
  }*/
}

/*
int main(){
	string path = "record1.wav";
	vector<int16_t> samples = read(path);
	std::cout<<samples.size()<<std::endl;

	std::vector<float> sileroBuffer_;
	double sileroVadProbability_ = 0.0;
	
	for (unsigned long i = 0; i < samples.size(); i++) {
		//frame.push_back(value);
		//leadingBuffer_.push_back(value);
		sileroBuffer_.push_back((float)samples[i]/ (float)SHRT_MAX);
		//sileroBuffer_.push_back((float)samples[i]);
		//webrtcVadBuffer_.push_back(value);
		//sum += value * value;
	}

	double probability = 0.0;
	const int num_samples = samples.size();
	const float interval = 0.1;
	//const float interval = 0.03;
	const int sample_rate = 16000;
	const int sample_interval = interval * sample_rate;
	int frame_idx = 0;
	
	Vad avad;
	auto start_time = clock();
	for (int start = 0; start < num_samples; start += sample_interval) {
		int end = std::min(start + sample_interval, num_samples);

		// Convert to short
		std::vector<int16_t> data;
		data.reserve(end - start);
		for (int j = start; j < end; j++) {
			data.emplace_back(samples[j]);
		}
		bool rt = avad.process(data);
	}
	auto end_time = clock();
	printf("Proceed exit after %.3f seconds\n", static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC);
}
*/

/*
	size_t num_input_nodes = ortSession_->GetInputCount();
    size_t num_output_nodes = ortSession_->GetOutputCount();
	std::cout<<num_input_nodes<<std::endl;
	std::cout<<num_output_nodes<<std::endl;


	for (int i = 0; i < num_input_nodes; i++) {
		char* input_name = ortSession_->GetInputName(i, allocator);
		cout<<input_name<<endl;

		Ort::TypeInfo type_info = ortSession_->GetInputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();

		ONNXTensorElementDataType type = tensor_info.GetElementType();
        printf("Input %d : type=%d\n", i, type);

		auto input_node_dims = tensor_info.GetShape();
        printf("Input %d : num_dims=%zu\n", i, input_node_dims.size());

        for (int j = 0; j < input_node_dims.size(); j++)
        {
            printf("Input %d : dim %d=%jd\n", i, j, input_node_dims[j]);
            if (input_node_dims[j] < 1){
                bool dynamic_flag  = true;
            }
        }
	}

	for(int i=0;i <num_output_nodes;i++){
		char* output_name = ortSession_->GetOutputName(i, allocator);
		cout<<output_name<<endl;
	}



*/
