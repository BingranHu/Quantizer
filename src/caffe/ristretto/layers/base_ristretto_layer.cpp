#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include "ristretto/base_ristretto_layer.hpp"

float my_pow(int q);

namespace caffe {

template <typename Dtype>
BaseRistrettoLayer<Dtype>::BaseRistrettoLayer() 
{
	init_flag_bias_ = 0;
	bw_params_bias_ = 0;
	fl_params_bias_ = 0;
}

template <typename Dtype>
void BaseRistrettoLayer<Dtype>::QuantizeLayerOutputs_cpu(Dtype* data, const int count) 
{
	Trim2FixedPoint_cpu(data, count, bw_layer_out_, fl_layer_out_, is_sign_out,0);
}

template <typename Dtype>
void BaseRistrettoLayer<Dtype>::QuantizeWeights_cpu(
	vector<shared_ptr<Blob<Dtype> > > weights_quantized,const bool bias_term) 
{
	
	Trim2FixedPoint_cpu(weights_quantized[0]->mutable_cpu_data(),
					weights_quantized[0]->count(),
					bw_params_, fl_params_,1,1);
	if (bias_term) 
	{
		//geyijun@2017-04-01
		//��bias ҲҪ����
		if(init_flag_bias_ == 0)
		{
			//ע��::: ���������bias  ����
			Dtype *bias_data =  weights_quantized[1]->mutable_cpu_data();
			Dtype max_bias = 0;
			for(int i=0;i<weights_quantized[1]->count();i++)
			{
				max_bias = (fabs(bias_data[i])>max_bias)?fabs(bias_data[i]):max_bias;
			}
			bw_params_bias_ = bw_params_;
			int il  = (int)ceil(log2(max_bias)+1);
			fl_params_bias_ = bw_params_bias_-il;
			init_flag_bias_ = 1;
		}
		Trim2FixedPoint_cpu(weights_quantized[1]->mutable_cpu_data(),
						weights_quantized[1]->count(), 
						bw_params_bias_, fl_params_bias_, 1,1);
	}
}
//������������
//��generator�ж�Ȩֵ��������������ʱ�������������
//������runner �����ʱ����Ϊֱ�����������㣬���Բ��ܽ�����������Ĳ���
template <typename Dtype>
void BaseRistrettoLayer<Dtype>::Trim2FixedPoint_cpu(Dtype* data, const int cnt, const int bit_width,int fl,int is_sign,int needround)
{
	// Saturate data
	Dtype max_data = 0;
	Dtype min_data = 0;
	if(is_sign)
	{
		max_data = (my_pow(bit_width - 1) - 1) * my_pow(-fl);
		min_data = -my_pow(bit_width - 1) * my_pow(-fl);
	}
	else
	{
		max_data = (my_pow(bit_width) - 1) * my_pow(-fl);
		min_data = 0;
	}
	for (int index = 0; index < cnt; ++index) 
	{
		//��������λһ��
		data[index] = std::max(std::min(data[index], max_data), min_data); 
		// Round data	
		data[index] /= my_pow(-fl);
		//geyijun@20170412
		//���������νض�(�Ը������������)
		//data[index] = (needround == 1)?round(data[index]):(int)(data[index]);
		data[index] = (needround == 1)?round(data[index]):floor(data[index]);
		data[index] *= my_pow(-fl);
	}
}

//������Ҫ������Щģ�庯��
template BaseRistrettoLayer<float>::BaseRistrettoLayer();
template void BaseRistrettoLayer<float>::QuantizeWeights_cpu(vector<shared_ptr<Blob<float> > > weights_quantized,const bool bias_term);
template void BaseRistrettoLayer<float>::QuantizeLayerOutputs_cpu(float* data,const int count);
template void BaseRistrettoLayer<float>::Trim2FixedPoint_cpu(float* data,const int cnt, const int bit_width,int fl,int is_sign,int needround);

template BaseRistrettoLayer<double>::BaseRistrettoLayer();
template void BaseRistrettoLayer<double>::QuantizeWeights_cpu(vector<shared_ptr<Blob<double> > > weights_quantized,const bool bias_term);
template void BaseRistrettoLayer<double>::QuantizeLayerOutputs_cpu(double* data,const int count);
template void BaseRistrettoLayer<double>::Trim2FixedPoint_cpu(double* data,const int cnt, const int bit_width,int fl,int is_sign,int needround);

}  // namespace caffe

