#!/usr/bin/env python
# -*- coding: cp936 -*-

from LocationBuilderImpl.LocationBuilderSquare import *
from GPHelper.HelperFunctions import *
from GPHelper.Envelope import *

def EnumLocationBuilders():
	return ["Eight-Neighbor-Square", "Four-Neighbor-Square", "Triangular", "Hexagon"]

class ArcGISPointsBuilder:
	"""
	�㹹������
	����ָ�����㷨��һ���ķ�Χ�ڹ���һ���й��ɵĵ㼯�����������Ҫ�����С�
	"""
	def __init__(self, boundaryFeatcls):
		"""
		���������캯����
		������
			boundaryFeatcls���߽�Ҫ�������ƣ�Ϊ��Ҫ���ࡣָ�������ɵ�ķ�Χ��
		"""
		self.ptsBuilders_ = {
			"Square" : PointsBuilderAlgoSquare(),
			"Triangular": PointsBuilderAlgoTri(),
			"Hexagon" : PointsBuilderAlgoHex()
		}
		self.boundaryProp_ = get_featureclass_properties(boundaryFeatcls)
		self.curalgo_ = None
		
	def _GetEnvelope(self):
		"""
		��������ñ߽�Ҫ����İ�Χ�С�����ȷ�����ɵ�����귶Χ��
		����ֵ��
			env��Envelop��ʾ�����μ�Helper.Envelop
		"""
		agsEnv = self.boundaryProp_.lsEnvelope
		env = Envelope(agsEnv[0], agsEnv[1], agsEnv[2], agsEnv[3])
		return env
	
	#ptsBuilderAlgo: Name of points builder algorithm. eg: "Square" or "Triangle"
	def Build(self, ptsBuilderAlgoName, algoParams):
		"""
		��������ָ����Χ��ʹ��ָ�����㷨����һ��㡣
		������
			ptsBuilderAlgo��
				�����㷨�����ơ�����"Square"�������ڹ��������ε�����
			algoParams��
				���빹���㷨�Ĳ������μ�������㷨��
				һԪ����ֱ�Ӵ��룬��Ԫ������Ԫ�顢�б�����ֵ����ʽ���롣
				������Square�㷨�У�algoParamsΪ������������ʾ��֮���ˮƽ/��ֱ�����
		"""
		self.curalgo_ = self.ptsBuilders_[ptsBuilderAlgoName]
		self.curalgo_.Build(self._GetEnvelope(), algoParams)
		return
	
	def GetPoints(self):
		"""
		��������ȡȫ���㡣
		����ֵ�����б���ʽ��[(x0, y0), (x1, y1), ...]
		ע�⣺
			���ܳ�����(x0,y0),(x1, y1)...��˳����б��棬
			����ʵ����Ҫ�����в�һ������������˳����д洢�����OID��
			ͬʱ��ȡҲû�а취��֤�Ͱ���OID��˳���ȡ��
			OIDҲ���ܱ�֤�����������ġ�
			��˽���һ��Ϣ���־û�ʹ��ʱ��Ҫ������
		"""
		return self.curalgo_.GetPoints()
	
	def Save(self, oFeatcls):
		"""
		�����������ɵĵ�Ҫ�ش洢��Ҫ������
		������
			oFeatcls��
				����ĵ�Ҫ����ȫ·��
		���أ�
			����ĵ�Ҫ����ȫ·����
		"""
		sPath, sName = os.path.split(oFeatcls)
		return create_point_fc(sPath, sName, self.GetPoints(), self.boundaryProp_)
		
	
