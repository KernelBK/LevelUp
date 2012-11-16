#!/usr/bin/env python
# -*- coding: cp936 -*-

from HelperFunctions import *
from xml.dom import *

class WeightCombinor:
	def __int__(self, weightFile):
		self._LoadWeights(weightFile)
		return
	
	def GetItems(self):
		"""
		���������Ȩ���ļ��ļ�Ȩ�����ơ�
		����ֵ��
			[weightItemName0, ...]
			weightItemName0Ϊ�ַ���
		"""
		return self.weights_.keys()
	
	def Calculate(self, wArray):
		"""
		���������ݴ����ģ��ֵ��������Ȩ����Ϣ���յ�λ��Ȩ��͡�
		������
			wArray������ĸ������Ҫ��ģ��ֵ��
			��ʽ��
				{featclsName: {ptId: fuzzyValue, ...} }
		"""
		self._CombineWeights(wArray)
	
	def GetTotalFuzzys(self):
		"""
		����������ģ��ֵ��Ȩ�͡�
		����ֵ��
			��ʽ {ptId: ptTotalFuzzy, ... }
			ptId Ϊint��ptTotalFuzzyΪDouble
		"""
		return self.totalFuzzys_
	
	def SaveToFile(self, oFile):
		"""
		��������ģ��ֵ
		"""
		f = open(oFile, "w")
		maxid = len(self.totalFuzzys_)
		f.write(str(maxid))
		for id in range(1, maxid + 1):
			f.write(self.totalFuzzys_[id])
		f.close()
	
	def _LoadWeights(self, weightFile):
		self.weights_ = {}
		wF = open(weightFile)
		weightStrs = wF.readlines()
		self.weights_ = dict(weightStrs)
		wF.close()
		
	def _CombineWeights(self, wArray):
		self.totalFuzzys_ = {}
		for featClsName in wArray:
			if self.weights_.has_key(featClsName):
				curWeight = float(self.weights_[featClsName])
				for ptId, ptFuzzy in wArray[featClsName].iteritems():
					curFuzzy = self.totalFuzzys_[int(ptId)]
					if not curFuzzy:
						curFuzzy = 0.0
					curFuzzy += curWeight * ptFuzzy
					self.totalFuzzys_[ptId]  = curFuzzy
	
	def _CheckIDContinurous(self):
		maxid = max(self.totalFuzzys_.keys())
		idcnt = len(self.totalFuzzys_.keys())
		if maxid != idcnt:
			print "Error"
			return False
		return True
	
class ArcGISWeightCombinor(WeightCombiner):
	def __init__(self, weightFile):
		WeightCombiner.__init__(weightFile)
		
	def SaveToFeatureClass(self, oFeatcls, field = "Fuzzy_Grade"):
		if pGP.Exists(oFeatcls):
			fcprop = get_featureclass_properties(oFeatcls)
		if not (field in fcprop.dcFields):
			pGP.AddField_management(oFeatcls, field, "DOUBLE")
			
		#Update
		rows = pGP.UpdateCursor(oFeatcls)
		row = rows.next()
		while row:
			fid = int(row.GetValue(fcprop.sFID))
			row.SetValue(self.totalFuzzys_[fid])
			row = rows.next()
		del row
		del rows
			