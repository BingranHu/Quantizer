#ifndef CAFFE_DATA_LAYER_HPP_
#define CAFFE_DATA_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/data_reader.hpp"
#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"
#include "caffe/layer.hpp"
#include "caffe/layers/base_data_layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"

namespace caffe {

template <typename Dtype>
class DataLayer : public BasePrefetchingDataLayer<Dtype> {
 public:
  explicit DataLayer(const LayerParameter& param);
  virtual ~DataLayer();
  virtual void DataLayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  // DataLayer uses DataReader instead for sharing for parallelism
  virtual inline bool ShareInParallel() const { return false; }
  virtual inline const char* type() const { return "Data"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

 protected:
  virtual void load_batch(Batch<Dtype>* batch);

  DataReader reader_;
};




//geyijun@2016-07-21
/**
 * �Զ������������
 *
 */
class Point 
{
public:
	Point(){m_x = 0;m_y = 0;}
	Point(int x,int y){m_x = x;m_y = y;}
	virtual ~Point(){};
	int m_x ;
	int m_y ;
};
class FourPointArea
{
public:
	FourPointArea()
	{
		m_pset[0].m_x = 0;
		m_pset[0].m_y = 0;
		m_pset[1].m_x = 0;
		m_pset[1].m_y = 0;
		m_pset[2].m_x = 0;
		m_pset[2].m_y = 0;
		m_pset[3].m_x = 0;
		m_pset[3].m_y = 0;
	};
	FourPointArea(int p0_x,int p0_y,int p1_x,int p1_y,int p2_x,int p2_y,int p3_x,int p3_y)
	{
		m_pset[0].m_x = p0_x;
		m_pset[0].m_y = p0_y;
		m_pset[1].m_x = p1_x;
		m_pset[1].m_y = p1_y;
		m_pset[2].m_x = p2_x;
		m_pset[2].m_y = p2_y;
		m_pset[3].m_x = p3_x;
		m_pset[3].m_y = p3_y;
	};
	virtual ~FourPointArea(){};
	bool GetBoundRect(Point &lt,Point &rb)//����Ӿ���
	{
		lt.m_x = (m_pset[0].m_x < m_pset[3].m_x) ? m_pset[0].m_x : m_pset[3].m_x ;
		lt.m_y = (m_pset[0].m_y < m_pset[1].m_y) ? m_pset[0].m_y : m_pset[1].m_y ;
		rb.m_x = (m_pset[2].m_x > m_pset[1].m_x) ? m_pset[2].m_x : m_pset[1].m_x ;
		rb.m_y = (m_pset[2].m_y > m_pset[3].m_y) ? m_pset[2].m_y : m_pset[3].m_y ;		
		return true;
	};
	Point m_pset[4];
};

template <typename Dtype>
class MyDataLayer : public BasePrefetchingDataLayer<Dtype> {
 public:
  explicit MyDataLayer(const LayerParameter& param)
      : BasePrefetchingDataLayer<Dtype>(param) {}
  virtual ~MyDataLayer();
  virtual void DataLayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual inline const char* type() const { return "MyData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  //geyijun@2016-12-14
  //����һ�������������������IOU �õ�
  //top[0]->data 			shape=NxCxHxW
  //��ǩģʽlable_mode
  //top[1]->label	shape=Nx1xHxW
  //top[1]->label	shape=Nx4 (��Ӿ��ε�����left top right bottom)
  virtual inline int ExactNumTopBlobs() const { return 2; }	

 protected:
  shared_ptr<Caffe::RNG> prefetch_rng_;
  virtual void ShuffleImages();
  virtual void load_batch(Batch<Dtype>* batch);
  virtual void  build_label(FourPointArea plate_area,int width,int height,Dtype* label);
  virtual bool get_plate_area(const string& cfgfile,FourPointArea & area);
  vector<std::pair<std::string, std::string> > lines_;
  int lines_id_;
  //geyijun@2016-12-19
  //Ϊ�˼ӿ�ȡ�����ٶ�(ֱ��Ӱ��ѵ�����ٶ�)
  std::map<string, FourPointArea> plate_area_map_;
};
}  // namespace caffe

#endif  // CAFFE_DATA_LAYER_HPP_
