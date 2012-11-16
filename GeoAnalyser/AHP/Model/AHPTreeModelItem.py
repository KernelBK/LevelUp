#!/usr/bin/env python
# -*- coding: cp936 -*-

from PyQt4 import QtGui, QtCore

class AHPTreeModelItem:
	def __init__(self, parentItem = None):
		self.ahpNode_  = None
		self.parentItem_ = parentItem
		self.childItems_ = []
	
	def IsValid(self):
		return self.ahpNode_ != None
	
	def ParentItem(self):
		return self.parentItem_
	
	def AddChildItem(self, childItem, pos = None):
		"""
		Ϊ��ǰ�����һ�����
		���ı�Node�Ĳ㼶�ṹ
		"""
		if pos == None:
			return self.childItems_.append(childItem)
		else:
			return self.childItems_.insert(pos, childItem)
		
	def RemoveChildItem(self, childItem):
		"""
		Ϊ��ǰ��ɾ��һ�����
		���ı�Node�Ĳ㼶�ṹ��
		"""
		return self.childItems_.remove(childItem)
	
	def ChildItem(self, row):
		return self.childItems_[row]
	
	def ChildItemCount(self):
		return len(self.childItems_)
	
	def SwapChildItemById(self, lhsId, rhsId):
		self.ahpNode_.SwapChildById(lhsId, rhsId)
		tmpChildItem = self.childItems_[lhsId]
		self.childItems_[lhsId] = self.childItems_[rhsId]
		self.childItems_[rhsId] = tmpChildItem
		self.Update()
		return
	
	def ColumnCount(self):
		return len(self.data_)
	
	def SetData(self, dataList):
		self.data_ = map(QtCore.QVariant, dataList)

	def Update(self):
		"""
		���µ�ǰ�ڵ�
		"""
		self.ahpNode_.Update()
		indexList = map(self.ahpNode_.GetPropertyByName, self.ahpNode_.GetPropertyNames())
		self.data_ = map(QtCore.QVariant, indexList)
	
	def UpdateTree(self):
		"""
		�����Ե�ǰ�ڵ�Ϊ���ڵ��Item��
		"""
		self.Update()
		for childItem in self.childItems_:
			childItem.UpdateTree()
	def SetAHPTree(self, rootNode):
		"""
		����AHPTree��
		��������ǰItem�Ĳ㼶�ṹʹ֮��AHPTreeһ�¡�
		"""
		self.ahpNode_ = rootNode
		self.Update()
		
		self.childItems_ = []
		for childAHPTreeNode in rootNode.GetChildren():
			childItem = AHPTreeModelItem(self)
			childItem.SetAHPTree(childAHPTreeNode)
			self.childItems_.append(childItem)
	
	def Data(self, column):
		return self.data_[column]
		
	def ParentItem(self):
		return self.parentItem_
	
	def Row(self):
		if self.parentItem_:
			parentItem  = self.parentItem_
			return parentItem.childItems_.index(self)
		return 0
	
	def GetTitles(self):
		return map(QtCore.QVariant, self.ahpNode_.GetIndexNames())
	
	def SetIndexById(self, indexId, value):
		if not self.ahpNode_: return False
		self.ahpNode_.SetPropertyById(indexId, value)
		self.Update()
		return True
	
	def SetIndexByName(self, indexName, value):
		if not self.ahpNode_: return False
		self.ahpNode_.SetPropertyByName(indexName, value)
		self.Update()
		return True