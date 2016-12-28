#include <vector>

#include "ristretto/base_ristretto_layer.hpp"
#include "caffe/filler.hpp"

namespace caffe {

template <typename Dtype>
DataRistrettoLayer<Dtype>::DataRistrettoLayer(
	const LayerParameter& param) : DataLayer<Dtype>(param),
	BaseRistrettoLayer<Dtype>()
{
	this->bw_params_ = this->layer_param_.quantization_param().bw_params();
	this->fl_params_ = this->layer_param_.quantization_param().fl_params();
	this->bw_layer_out_= this->layer_param_.quantization_param().bw_layer_data();
	this->fl_layer_out_= this->layer_param_.quantization_param().fl_layer_data();
	this->is_sign_out= this->layer_param_.quantization_param().is_sign_data();
	printf("DataRistrettoLayer------------------------>111\n");
}

template <typename Dtype>
void DataRistrettoLayer<Dtype>::load_batch(Batch<Dtype>* batch)
{
	//���ø����ǰ������
	DataLayer<Dtype>::load_batch(batch);
	//�Խ�������������
	this->QuantizeLayerOutputs_cpu(batch->data_.mutable_cpu_data(), batch->data_.count());
}

INSTANTIATE_CLASS(DataRistrettoLayer);
REGISTER_LAYER_CLASS(DataRistretto);

}  // namespace caffe

