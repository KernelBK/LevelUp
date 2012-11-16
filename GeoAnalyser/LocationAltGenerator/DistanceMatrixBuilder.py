#!/usr/bin/env python
# -*- coding: cp936 -*-

from HelperFunctions import *
import networkx

class NetworkDistanceMatrixBuilder:
	"""
	������������ľ��������㡣
	"""
	def __init__(self):
		self.graph_ = networkx.XGraph()
		self.distMat_ = {}
		self.ptCnt_ = 0
		return
	
	def GetFeatureCount(self, featcls):
		"""
		��������ȡҪ�����Ҫ��������
		ע�⣺���㷨�Ƕ�ȡ���е�Ҫ�ز������ģ����Ч�ʽϵ͡�
		������
			featcls����Ҫͳ��Ҫ��������Ҫ���ࡣ
		����ֵ��Ҫ�ص�������������
		"""
		retCnt = 0
		rows = pGP.SearchCursor(featcls)
		row = rows.next()
		while row:
			retCnt += 1
			row = rows.next()
		del rows
		del row
		
		return retCnt
		
	def Build(self, inFeatcls):
		"""
		������ʹ��һ����Ҫ���ཨ���������
		ע�⣺
			precondition��
				SRCPTID��DESTPTIDԴ����ͬһ����Ҫ���ࣻ
				��Ҫ������Ҫ�ص�OID��1��ʼ��n��nΪ�����Ŀ��
				SRCPTID��DESTPTID���߶ζ˵��Ƕ�Ӧ��
			�����Ҫ��������Ҫ���๹�ɵ�ͼ���й������벻��ͨ���������
		������
			inFeatcls����Ҫ��������
		"""
		self._LoadFeatures(inFeatcls)
		self.nodeList_ = self.graph_.nodes()
		for ptId in self.graph_.nodes():
			self.distMat_[ptId] = networkx.path.single_source_dijkstra_path_length(self.graph_, ptId)
		return 
	
	def Save(self, ptCnt, oFileName):
		"""
		������
			�����������ļ��С�
			�ļ��������ṩ�˵��������
			���������Ϊn�����ļ�����n��n�е���ʽ����Ϊһ��n*n��˫���ȸ������
			�к�Ϊ��㣬�к�Ϊ�յ㡣
			����ֹ�����ͬ�������Ϊ0������ֹ�����ͨ·����ͨ·��С��Ϊ1e300.
		ע�⣺
			����ͨ·�ľ����趨Ϊ1e300���Կ��ܶԽ�������ϴ��Ӱ�졣
			precondition
				ptCntΪ��Ӧ��Ҫ�����Ҫ��������ͬ��
		������
			ptCnt��ͼ�нڵ���������Ӧ��Ҫ�������������
			oFileName����������ļ���
		"""
		f = open(oFileName, "w")
		
		f.write("%d" % ptCnt)
		for srcPtId in range(1, ptCnt + 1):
			for destPtId in range(1, ptCnt + 1):
				isHasKey = False
				if self.distMat_.has_key(srcPtId):
					if self.distMat_[srcPtId].has_key(destPtId):
						f.write( "%24.16e" % self.distMat_[srcPtId][destPtId])
						isHasKey = True
				if not isHasKey:
					f.write("%24.16e" % 1e200)
			f.write("\n")
		f.close()
		
		return
	
	def _LoadFeatures(self, lineFeatcls):
		"""
		��������ȡ��Ҫ���࣬��������Ϊͼ�ı߼���ͼ�С�
		ע�⣺��Ҫ������Ҫ����һ����Schema��
		���룺
			lineFeatcls����Ҫ�������ơ�
		"""
		rows = pGP.SearchCursor(lineFeatcls)
		row = rows.next()
		
		while row:
			self.graph_.add_edge(int(row.SRCPTID), int(row.DESTPTID), row.Shape.Length)
			row = rows.next()
			
		del rows
		del row