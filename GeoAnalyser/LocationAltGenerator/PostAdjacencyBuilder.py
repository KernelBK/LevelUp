#!/usr/bin/env python
# -*- coding: cp936 -*-

from PostAdjacencyBuilderImpl.DistanceThreshold import *
from GPHelper.HelperFunctions import *

pGP.OverWriteOutput = True

class ArcGISPostAdjBuilder:
	"""����ArcGIS���ڽ���Ϣ��������ʵ�ֿ�����Ӳ�ͬ���ڽӾ����㷨��
	�ڽ���Ϣ��ʹ��һ����Ҫ������Ϊ���롣
	�����������ڽӹ�ϵ����������������Ϊ�˵㹹��һ���߶Σ������浽һ����Ҫ�����С�
	"""
	def __init__(self, inFeatcls, adjBuilderAlgo = DistanceThreshold()):
		"""
		���������캯����
		������
			inFeatcls��
				���ڹ����ڽӾ���ĵ�Ҫ���������·����
			adjBuilderAlgo��
				�����㷨���㷨��PostAdjBuilder_Algo�ļ���ʵ�֡�
		"""
		self.inFeatcls_ = inFeatcls
		self.adjBuilderAlgo_ = adjBuilderAlgo
		self.inFeatclsProp_ = get_featureclass_properties(inFeatcls)
		
	def _ReadPoints(self):
		"""
		������
			��Ҫ�����ж�ȡ����Ϣ��
		ע�⣺
			self.pnts_��ʽΪ[(x0, y0), (x1, y1), ...]
		"""
		self.pnts_ = get_feature_points(self.inFeatcls_)
		
	def _BuildPointIdListFromNeighbors(self, neighbors):
		"""
		�����������ڽ���Ϣ�����ID�б�
		����ֵ��
			���б���ʽ��[(ptID0, ptID1), (ptID2, ptID3), ... ]
			ͬһԪ�������Ԫ�����ڽӹ�ϵ������ (ptID0, ptID1) ��ʾptID0��ptID1���ڽӹ�ϵ��
		"""
		retPtIdList = []
		for srcPtId, destPtIds in neighbors.iteritems():
			for destPtId in destPtIds:
				retPtIdList.append( (srcPtId, destPtId) )
		return retPtIdList
	
	def _BuildPointCoordListFromIdList(self, idList):
		"""
		���������õ��б��������б�
		ע�⣺
			precondition��
				��Ҫ�ص�OID��1��ʼ������������ģ�
				��ȡ��ʱ��1��ʼ��ȡ��OIDΪi��Ҫ������һ�������pnts_[i-1]����
			postcondition��
				ͬprecondition��
		������
			idList��ͬ_BuildPointIdListFromNeighors�ķ���ֵ��
		���أ�
			���ص�������б���ʽ��[ ((ptCoordX0, ptCoordY0), (ptCoordX1, ptCoordY1))]
			ptCoordX/Yn��ӦptIDn��X/Y���ꡣ
			�μ�_BuildPointIdListFromNeighbors�ķ���ֵ��Ϣ��
		"""
		retPtCoords = []
		for idPair in idList:
			retPtCoords.append(self.pnts_[idPair[0] - 1])
			retPtCoords.append(self.pnts_[idPair[1] - 1])
		return retPtCoords
	
	def _UpdateLineEndIds(self, oFeatcls):
		"""
		������
			�������ߵĶ˵���Ϣ��
		ע�ͣ�
			�ڽ�������Ϣ�ı���ʹ����create_line_fc������
			�������������߶α�����û�б���˵���Ϣ��
			����������һ������飬���ڽ��߶εĶ˵㱣�浽Ҫ����Ϣ�С�
			Precondition��
				���߶�Ҫ�ص�д˳����֮ǰ���漸����Ϣʱ��д˳����ͬ��
		������
			oFeatcls����Ҫ���µ�Ҫ����ȫ·������Save�е�Ҫ������ͬ��
		"""
		pGP.RefreshCatalog(oFeatcls)
		
		if pGP.Exists(oFeatcls):
			pGP.AddField_management(oFeatcls, "SRCPTID", "long", 9)
			pGP.AddField_management(oFeatcls, "DESTPTID", "long", 9)
			
		rows = pGP.UpdateCursor(oFeatcls)
		row = rows.next()
		pntIdCur = 0
		while row:
			row.SetValue("SRCPTID", self.neighborPntIds_[pntIdCur][0])
			row.SetValue("DESTPTID", self.neighborPntIds_[pntIdCur][1])
			rows.UpdateRow(row)
			pntIdCur += 1
			row = rows.next()
		del row
		del rows
		return
	
	def Build(self, params):
		"""
		�����������ڽ���Ϣ��
		������
			paramsΪ��������Ԫ�顢�ֵ䡢�����б����ʽ�ṩ��һԪ�Ĳ�������Ҫд��Ԫ�顣
		"""
		self._ReadPoints()
		self.adjBuilderAlgo_.SetPoints(self.inFeatcls_)
		self.adjBuilderAlgo_.Build(params)
		neighbors = self.adjBuilderAlgo_.GetAdjInfos()
		self.neighborPntIds_ = self._BuildPointIdListFromNeighbors(neighbors)
		self.neighborPnts_ = self._BuildPointCoordListFromIdList(self.neighborPntIds_)
		return
	
	def Save(self, oFeatcls = None):
		"""
		������
			��������ϵ��ڽ���Ϣ���浽��Ҫ�������С�
		������
			oFeatcls��
				���Ҫ�������ơ�
				��ѡ��������������û�����Ϊ�գ�
				����������ĵ�Ҫ�������ƹ����������Ҫ�������ơ�
				�����Ҫ����Ϊ$(point), ��ôĬ�ϵ���Ҫ��������Ϊ$(point)_network��
		����ֵ��
			�������Ҫ��������ơ�
		"""
		if oFeatcls == None:
			oFeatclsPath, inFeatclsName = os.path.split(self.inFeatcls_)
			oFeatclsName = inFeatclsName + "_network"
			oFeatcls = os.path.join(oFeatclsPath, oFeatclsName)
			
		oFeatclsPath, oFeatclsName = os.path.split(oFeatcls)
		
		create_line_fc(
			oFeatclsPath,
			oFeatclsName,
			self.neighborPnts_,
			self.inFeatclsProp_
			)
		
		self._UpdateLineEndIds(oFeatcls)
		return oFeatcls
	
	def GetAdjMat(self):
		return