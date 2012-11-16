#!/usr/bin/env python
# -*- coding: cp936 -*-

import math

class PointsBuilderAlgoSquare:
	"""
	���������Envelop�������������������εĵ㼯�����ṩԤ������ڽӱ�
	"""
	def __init__(self):
		"""
		��������ʼ��������
		"""
		self.pts_ = None
		self.adjMat_ = None
		self.adjBuilders_ = {
			"4-Link": self._Adj4Link,
			"8-Link": self._Adj8Link
			}
		
	def _MakePts(self, env, dist):
		"""
		������
			�����ɳ����ʵ�֡�
		������
			env��Envelop����ָ���˵�����ɷ�Χ�������Χ�У��������ɵĵ㲻�ᳬ���÷�Χ��
			dist�������������ڵ�֮�������ϵľ��롣
		"""
		dMaxX = env.MaxX
		dMinX = env.MinX
		dMaxY = env.MaxY
		dMinY = env.MinY
		fResolution = dist
		
		lNumXs = int(math.floor((dMaxX - dMinX) / fResolution)) + 1
		lNumYs = int(math.floor((dMaxY - dMinY) / fResolution)) + 1
	
		# create a list of points
		pPnts = []
		for iY in range (0, lNumYs):
			dY = dMinY + iY * fResolution
			dX = 0
			for iX in range (0, lNumXs):
				dX = dMinX + iX * fResolution
				p = (dX, dY)
				pPnts.append(p)
		
		self.pts_ = pPnts
	
	def _MakeAdjMat(self, adjBuilderName):
		"""
		�������ڽӾ���Ԥ���졣���޿��õ�ʵ�֡�
		������
			adjBuilderName:
				�ڽӱ�Ԥ������������
		"""
		pFunc = self.AdjBuilders_[adjBuilderName]
		self.adjMat_ = pFunc()
		
	def _Adj4Link(self):
		"""
		������
			�ķ�����ڽӱ�Ԥ��������ʵ�֡���δʵ�֡�
		"""
		return
		
	def _Adj8Link(self):
		"""
		������
			�˷�����ڽӱ�Ԥ��������ʵ�֡���δʵ�֡�
		"""
		return
	
	def GetAdjMat(self):
		"""
		����������ڽӾ���������δʵ��Ԥ����������˸÷������Ƿ��ؿա�
		"""
		return self.adjMat_
	
	def GetPoints(self):
		"""
		�������������ɵĵ��б�
		����ֵ��
			[ (x0, y0), (x1, y1), ... ]
		"""
		return self.pts_
	
	def GetAdjMatAlgos(self):
		"""
		��������ÿ��õ��ڽӱ��Ԥ�����㷨�����б��б��е����ƿ�����Build�Ĳ�����
		����ֵ��
			���õ�Ԥ�����㷨�����б�
			���磺["algo name1", "algo name2"]
		"""
		return self.adjBuilders_.keys()
		
	def Build(self, env, dist, adjMatName = None):
		"""
		��������ָ���ķ�Χ�ڰ���һ���ļ����������������еĵ㼯��
		������
			env��Envelop��ʵ����ָ���˹����ķ�Χ��
			dist����֮��ļ�������������ڵ�ľ��룩
			adjMatName���ڽӱ����������ơ�
		����ֵ���ޡ������ĵ㼯���ڽӱ�ʹ��Get������ȡ��
		"""
		self._MakePts(env, dist)
		if adjMatName and self.adjBuilders_.has_keys():
			self._MakeAdjMat(adjMatName)

class PointsBuilderAlgoTri:
	"""
	���������Envelop������ױ���Env�ױ�ƽ�е��������μ���
	"""
	def __init__(self):
		self.pts_ = None
	
	def _MakePts(self, env, dist):
		"""
		��������ָ����Χ�����ɵ�
		������
			env��envelop����ָ��������ɷ�Χ
			dist�������εı߳�
		��ע�����������εױ�ƽ��env�ĵױߣ��и�Ϊsqrt(3) / 2���ı߳�
		"""
		
		rowEdgeRatio = math.sqrt(3.0) / 2.0 #�иߺͱ߳��ı�ֵ
		rowHeight = dist * rowEdgeRatio
		
		#�������ͽϿ��еĵ���
		rowPtCount = int(math.floor((env.MaxY - env.MinY) / rowHeight)) + 1
		majColPtCount = int(math.floor((env.MaxX - env.MinX) / dist)) + 1
		
		pnts = []
		
		for iRow in range(rowPtCount):
			curColPtCount = majColPtCount
			curOffset = 0.0
			if iRow % 2 == 1:
				curColPtCount = majColPtCount - 1
				curOffset = dist / 2.0
			for iCol in range(curColPtCount):
				curX = env.MinX + curOffset + iCol * dist
				curY = env.MinY + iRow * rowHeight
				pnts.append((curX, curY))
		
		self.pts_ = pnts
		
	def GetPoints(self):
		return self.pts_
	
	def Build(self, env, dist):
		"""
		��������ָ���ķ�Χ�ڰ���һ���ļ��������������η������еĵ㼯��
		������
			env��Envelop��ʵ����ָ���˹����ķ�Χ��
			dist�������εı߳�
		����ֵ���ޡ������ĵ㼯���ڽӱ�ʹ��Get������ȡ��
		"""
		self._MakePts(env, dist)
		#if adjMatName and self.adjBuilders_.has_keys():
			#self._MakeAdjMat(adjMatName)

class PointsBuilderAlgoHex:
	def __init__(self):
		self.pts_ = None
	
	def _MakePts(self, env, dist):
		#���������ڵȱ������εĻ�����ɸ�㡣
		
		rowEdgeRatio = math.sqrt(3.0) / 2.0 #�иߺͱ߳��ı�ֵ
		rowHeight = dist * rowEdgeRatio
		
		#�������ͽϿ��еĵ���
		rowPtCount = int(math.floor((env.MaxY - env.MinY) / rowHeight)) + 1
		majColPtCount = int(math.floor((env.MaxX - env.MinX) / dist)) + 1
		
		pnts = []
		
		for iRow in range(rowPtCount):
			curColPtCount = majColPtCount
			curOffset = 0.0
			clipPosOffset = 2
			
			if iRow % 2 == 1:
				curColPtCount = majColPtCount - 1
				curOffset = dist / 2.0
				clipPosOffset = 1
				
			for iCol in range(curColPtCount):
				if (iCol + clipPosOffset) % 3 != 0:
					curX = env.MinX + curOffset + iCol * dist
					curY = env.MinY + iRow * rowHeight
					pnts.append((curX, curY))
		
		self.pts_ = pnts
		
	def GetPoints(self):
		return self.pts_
	
	def Build(self, env, dist):
		self._MakePts(env, dist)