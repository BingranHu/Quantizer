#include <vector>

#include "ristretto/base_ristretto_layer.hpp"
#include "caffe/filler.hpp"

namespace caffe {

template <typename Dtype>
ConvolutionRistrettoLayer<Dtype>::ConvolutionRistrettoLayer(
	const LayerParameter& param) : ConvolutionLayer<Dtype>(param),
	BaseRistrettoLayer<Dtype>()
{
	this->bw_params_ = this->layer_param_.quantization_param().bw_params();
	this->fl_params_ = this->layer_param_.quantization_param().fl_params();
	this->bw_layer_out_= this->layer_param_.quantization_param().bw_layer_data();
	this->fl_layer_out_= this->layer_param_.quantization_param().fl_layer_data();
	this->is_sign_out= this->layer_param_.quantization_param().is_sign_data();
	printf("ConvolutionRistrettoLayer------------------------>111\n");
}

template <typename Dtype>
void ConvolutionRistrettoLayer<Dtype>::Forward_cpu(
	const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) 
{
	//printf("ConvolutionRistrettoLayer------------------------>Forward_cpu\n");
	//��Ȩֵ������������
	//float kernel_1 = *(this->blobs_[0]->mutable_cpu_data());
	this->QuantizeWeights_cpu(this->blobs_, this->bias_term_);
	//float kernel_2 = *(this->blobs_[0]->mutable_cpu_data());	
	//static int aaa  = 0;
	//if(aaa == 0)
	//{
	//	aaa = 1;
	//    printf("ConvolutionRistrettoLayer------------------------>debug[%f]--->[%f]\n",kernel_1,kernel_2);
	//}

	//���ø����ǰ������
	//ConvolutionLayer<Dtype>::Forward_cpu(bottom,top);
	ConvolutionLayer<Dtype>::Forward_gpu(bottom,top);

	//�Խ�������������
	//�Աȷ���bias  �Ĵ���ʽ����һ�»ᵼ��һ���Ĳ���
	/*
	if(this->layer_param_.name() == "conv3")
	{
		printf("ConvolutionRistrettoLayer------------------------>before--->src[%f][%f][%f][%f][%f][%f][%f][%f][%f][%f]\n",
												bottom[0]->data_at(0, 0,10,10),
												bottom[0]->data_at(0, 1,10,10),
												bottom[0]->data_at(0, 2,10,10),
												bottom[0]->data_at(0, 3,10,10),
												bottom[0]->data_at(0, 4,10,10),
												bottom[0]->data_at(0, 5,10,10),
												bottom[0]->data_at(0, 6,10,10),
												bottom[0]->data_at(0, 7,10,10),
												bottom[0]->data_at(0, 8,10,10),
												bottom[0]->data_at(0, 9,10,10));
		printf("ConvolutionRistrettoLayer------------------------>before--->dst[%f]\n",top[0]->data_at(0, 0,10,10));
		this->QuantizeLayerOutputs_cpu(top[0]->mutable_cpu_data(), top[0]->count());
		printf("ConvolutionRistrettoLayer------------------------>after--->src[%f][%f][%f][%f][%f][%f][%f][%f][%f][%f]\n",
												bottom[0]->data_at(0, 0,10,10),
												bottom[0]->data_at(0, 1,10,10),
												bottom[0]->data_at(0, 2,10,10),
												bottom[0]->data_at(0, 3,10,10),
												bottom[0]->data_at(0, 4,10,10),
												bottom[0]->data_at(0, 5,10,10),
												bottom[0]->data_at(0, 6,10,10),
												bottom[0]->data_at(0, 7,10,10),
												bottom[0]->data_at(0, 8,10,10),
												bottom[0]->data_at(0, 9,10,10));
		printf("ConvolutionRistrettoLayer------------------------>after--->dst[%f]\n",top[0]->data_at(0, 0,10,10));
	}
	else
	*/	this->QuantizeLayerOutputs_cpu(top[0]->mutable_cpu_data(), top[0]->count());
}

template <typename Dtype>
void ConvolutionRistrettoLayer<Dtype>::Forward_gpu(
	const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) 
{
	return Forward_cpu(bottom, top);
}

INSTANTIATE_CLASS(ConvolutionRistrettoLayer);
REGISTER_LAYER_CLASS(ConvolutionRistretto);

}  // namespace caffe

