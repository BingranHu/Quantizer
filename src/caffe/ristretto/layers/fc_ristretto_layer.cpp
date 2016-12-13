#include <vector>

#include "caffe/filler.hpp"
#include "caffe/layers/inner_product_layer.hpp"
#include "caffe/util/math_functions.hpp"
#include "ristretto/base_ristretto_layer.hpp"

namespace caffe {

template <typename Dtype>
FcRistrettoLayer<Dtype>::FcRistrettoLayer(const LayerParameter& param)
      : InnerProductLayer<Dtype>(param), BaseRistrettoLayer<Dtype>() 
{
	this->bw_params_ = this->layer_param_.quantization_param().bw_params();
	this->fl_params_ = this->layer_param_.quantization_param().fl_params();
	this->bw_layer_out_= this->layer_param_.quantization_param().bw_layer_data();
	this->fl_layer_out_= this->layer_param_.quantization_param().fl_layer_data();
	this->is_sign_out= this->layer_param_.quantization_param().is_sign_data();
}

template <typename Dtype>
void FcRistrettoLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) 
{
	//��Ȩֵ������������
	this->QuantizeWeights_cpu(this->blobs_, this->bias_term_);
	//���ø����ǰ������
	InnerProductLayer<Dtype>::Forward_cpu(bottom,top);
	//�Խ�������������
	this->QuantizeLayerOutputs_cpu(top[0]->mutable_cpu_data(), top[0]->count());
}

#ifdef CPU_ONLY
STUB_GPU(FcRistrettoLayer);
#endif

INSTANTIATE_CLASS(FcRistrettoLayer);
REGISTER_LAYER_CLASS(FcRistretto);

}  // namespace caffe