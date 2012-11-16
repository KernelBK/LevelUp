#!/usr/bin/env python
# -*- coding: cp936 -*-

from GPHelper.HelperFunctions import *

"""
�ڽӱ���������㷨ʵ���ļ���
��������������λ���깹���ڽӹ�ϵ��
"""

class DistanceThreshold:
	"""�ڵ��ڽӱ����������ڹ���ڵ���ڽ���Ϣ��
	ʹ��Set -> Build -> Get��˳���ȡ��Ϣ��
	"""
	def __init__(self):
		return
	
	def SetXYs(self, pntsX, pntsY):
		"""
		���������õ�λ��XY���ꡣ
		����ֵ���޷���ֵ
		������
			pntsX�����X���ꡣ��ʽΪ{ptID0: CoordX0, ptID1: CoordX1, ...}
						ptIDnΪ���ID��
						CoordXnΪ��ptIDn��X���ꡣ
			pntsY����ʽͬpntsX��
		"""
		self.pntsX_ = pntsX
		self.pntsY_ = pntsY
		
	def SetPoints(self, inFeatcls):
		"""
		���������õ�Ҫ���ࡣ
		����ֵ���޷���ֵ
		������
			inFeatcls����Ҫ�����ȫ·����
		"""
		self.pntsX_, self.pntsY_ = build_value_lists(inFeatcls)
	
	def Build(self, distLimit):
		"""
		�����������ڽӱ�
		����ֵ���޷���ֵ
		������
			distLimit��������ֵ������������С�ڸ�ֵ����Ϊ�����ڽӡ�
		"""
		self.pntsNeighbors_ = build_neighbor_list(self.pntsX_, self.pntsY_, distLimit)
	
	def BuildWithDist(self):
		"""
		�����������������Ϣ���ڽӱ�δʵ�֡�
		����ֵ����
		"""
		return
	
	# ����ڽӱ�
	# ���ص��ڽӱ��ʽ��
	def GetAdjInfos(self):
		"""
		��������ù�����ϵ��ڽӱ�
		����ֵ��
			����һ���ڽӱ���ʽ��{ sourceNodeId0 : [destNodeId0, destNodeId1, ...], ... }
			����sourceNodeId0��destNodeId0/1�����ڡ�	
		"""
		return self.pntsNeighbors_
	
	# �����ڽӾ�����ʱ�޷�ʹ�ã�
	def GetAdjMat(self):
		"""
		���������ش�������Ϣ���ڽӾ���δʵ��
		����ֵ����������Ϣ���ڽӾ���
		"""
		return