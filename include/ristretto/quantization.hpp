#ifndef QUANTIZATION_HPP_
#define QUANTIZATION_HPP_

#include "caffe/caffe.hpp"

using caffe::string;
using caffe::vector;
using caffe::Net;
using caffe::Blob;
using caffe::NetParameter;

/**
 * @brief Approximate 32-bit floating point networks.
 *
 * This is the Ristretto tool. Use it to generate file descriptions of networks
 * which use reduced word width arithmetic.
 */
class Quantization {
public:
  explicit Quantization(string model, string weights, string model_quantized,
      int iterations, double error_margin, string gpus,string quantize_cfg,
      string debug_out_float,string debug_out_trim);
  void QuantizeNet();
  //�޸�������������
  static void EditNetQuantizationParameter(NetParameter* param,
					  				vector<string> layer_names,
									vector<int> params_bw,vector<int> params_fl,
									vector<int> data_bw,vector<int> data_fl,
									vector<int> data_sign);
  
private:
  void CheckWritePermissions(const string path);
  void SetGpu();

  
  //ö�ٳ�������Ҫ������bw ���
  vector<int> GetValidBw(string cur_name);
  vector<vector<int> > CombineBwCfg(vector<vector<int> > before_result, vector<int> cur_in);
  void ParseBwCfg();

  //��������������
  float Float2FixTruncate(float val,int bw,int fl,int is_sign);		
  float CalcDataLoss(Blob<float>* blob,int bw,int fl,int is_sign);

  //����һ��bw����,���(���ֵ����Сֵ)��ʹ�á��ֲ����š���׼�������ò��is_sign��fl������
  void CalcFlSign(const int iterations,Net<float>* caffe_net);
  void CalcFlSign_ForLSTM(Net<float>* caffe_net);
  
  //����һ�����εľ���
  void CalcBatchAccuracy(const int iterations,Net<float>* caffe_net, float* accuracy,float* cur_accuracy,const int score_number );

  //�Ƚ��������õľ��ȴ�С
  int CompareBwCfg(vector<int>& bwcfg1,vector<int>& bwcfg2);
  void DumpAllBlobs2Txt(Net<float>* caffe_net,string dumpdir);
  
  //���ò���(�û������)
  string model_;
  string weights_;
  static string model_quantized_;
  int iterations_;
  double error_margin_;
  string gpus_;
  string quantize_cfg_;	//���������ļ�������ָ��ÿһ����õ�bw ö��ֵ

  //���������
  string debug_out_float_;
  string debug_out_trim_;
  
  //������Ϣ��bw.cfg �����ļ��л�ȡ��
  int cfg_conv_params_bw_; 	//�û����õľ����Ȩ�ص�λ��
  int cfg_ip_params_bw_;		//�û����õ�ȫ���Ӳ�Ȩ�ص�λ��
  int cfg_default_data_bw_;	//�û����õ�Ĭ�ϵ�λ��
  int cfg_auto_search_;		//�Ƿ��Զ���������  
  vector<vector<int> > cfg_valid_data_bw_;			//ö�ٳ�������Ч��bw����(�Զ�������)
  vector<vector<int> > calc_valid_data_fl_;			//��Ӧ�������ÿһ��bw ��fl 
  vector<vector<int> > calc_valid_data_sign_;		//��Ӧ�������ÿһ��bw ��s_sign
  vector<float> max_params_, max_data_, min_data_;//ͳ�����õ���ֵ
  vector<int> cfg_valid_data_bw_skip_;	//���Ա�־
 
  //ע��:  Ȩֵ������bw ֱ�Ӹ����û����õ�λ����
  //�̶�ʹ���з�����������fl  ֱ�Ӹ���max ����ͺ��ˡ�
  vector<int> calc_params_bw_;
  vector<int> calc_params_fl_;
 
  //��ѵ������float  �����ͳ�ƽ�����û��ֲ����Ŷ���
  vector<string> layer_names_;  //ÿһ�������(ע���net�е��в��net���һЩsplit��)
  
  //�ڼ��鼯��float  ����ķ�������Ϊȫ�����۵Ļ�׼��
  float test_score_baseline_;	
};	

#endif // QUANTIZATION_HPP_
