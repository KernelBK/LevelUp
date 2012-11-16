#!/usr/bin/env python
# -*- coding: cp936 -*-
from GPHelper.HelperFunctions import pGP

class ArcGISLocationClipFilter:
	"""
	����ArcGIS�Ĳ���ʵ�֡�
	"""
	def __init__(self, boundaryFeatcls):
		"""
		��������ʼ��������
		������
			boundaryFeatcls���߽���Ҫ���ࡣ
		"""
		self.boundaryFeatcls_ = boundaryFeatcls
		self.destFeatcls_ = None
		
	def Filtrate(self, srcFeatcls, destFeatcls = None):
		"""
		�������������Ҫ�����������趨�ı߽�Ҫ������вü������������Ҫ�����С�
		������
			srcFeatcls�����ü���Ҫ���ࡣ
			destFeatcls���ü��Ľ��Ҫ���ࡣ�������ΪNone�������Զ�Ϊ�����������
		���أ�
			�޷���ֵ�������Ҫ��ȡ���Ҫ�������ƣ�ʹ��GetClipped������
		"""
		# create an output feature class
		envelopeFC = "TEMP_Envelope"
		self.destFeatcls_ = destFeatcls
		if destFeatcls == None or destFeatcls == "":
			self.destFeatcls_ = srcFeatcls + "_filtered"
		
		pGP.MakeFeatureLayer_management(self.boundaryFeatcls_, envelopeFC, True)
		pGP.Clip_analysis(srcFeatcls, envelopeFC, self.destFeatcls_)
		pGP.Delete_management(envelopeFC)
		
	def GetFilteredFeatureClassName(self):
		return self.destFeatcls_