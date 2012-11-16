#!/usr/bin/env python
# -*- coding: cp936 -*-

import sys, os, math
from xml.dom.minidom import *
from copy import *

from GPHelper.HelperFunctions import * 
from GPHelper.FeatureClass import *
from GPHelper.Log import *
from GPHelper.Shape import *
from GPHelper.Workspace import *

from ProximityBuilderImpl.LinguisticTable import *

#ö�ٿ��õ�ProxmityBuilder
def EnumProximityBuilders():
	return ["Distance", "Fuzzy Grade"]#, "Arithmetic Level"]
	
def CreateProximityBuilder(algo, featCls):
	if algo == "Fuzzy Grade":
		return ArcGISFuzzyProximityBuilder(featCls, algo)
	if algo == "Distance":
		return ArcGISGeoProximityBuilder(featCls, algo)
	if algo == "Arithmetic Level":
		return ArcGISArithmeticLevelProximityBuilder(featCls, algo)
	if algo == "Geometric Level":
		return ArcGISGeometricLevelProximityBuilder(featCls, algo)
	if algo == "Manual Level":
		return ArcGISManualLevelProximityBuilder(featCls, algo)
	if algo == "Smooth Level":
		return ArcGISSmoothLevelProximityBuilder(featCls, algo)
	if algo == "Programmable":
		return ArcGISProgrammableLevelProximityBuilder(featCls, algo)
	return None

#�����ٽ���
class ArcGISGeoProximityBuilder:
	"""
	ʹ��ArcGIS�����ٽ�����Ϣ��
	"""
	def __init__(self, inFeatcls, distAlgo = None):
		"""
		���������캯����
		������
			inFeatcls������ĵ�Ҫ�����͡�
			distAlgo���ռ��������㷨����ʱ��ʵ�֡�
		"""
		self.distAlgo_ = distAlgo
		self.lingTbls_ = None
		
		self.inFeatcls_ = inFeatcls
		self.inFeatclsProp_ = get_featureclass_properties(inFeatcls)
		
		#Reschema the Dataset
		fuzzyField = "Fuzzy_Grade"
		if fuzzyField.upper() not in self.inFeatclsProp_.dcFields:
			if not self.inFeatclsProp_.dcFields.has_key(fuzzyField.upper()):
				pGP.AddField(inFeatcls, fuzzyField, "DOUBLE")
		
		return
	
	#���linguistic file��Ҫ�����Ƿ�ƥ�䡣
	def Check(self, lingTbl):
		lingVals = lingTbl.Data().values()
		vecWs = Workspace(os.path.dirname(self.inFeatcls_))
		rasWs = Workspace(os.path.dirname(os.path.dirname(self.inFeatcls_)))
		retErrors = []
		for lingVal in lingVals:
			if lingVal.FeatType == "Vector":
				if not vecWs.IsInclude(lingVal.FeatCls):
					retErrors.append("feature class \" %s \" is not exist!" % lingVal.FeatCls)
			if lingVal.FeatType == "Raster":
				if not rasWs.IsInclude(lingVal.FeatCls):
					retErrors.append("raster \" %s \" is not exist!" % lingVal.FeatCls)
		return retErrors
	
	def Build(self, lingTbl):
		"""
		������
			����linguistic file�������Ҫ���������е㵽��������Ҫ����ľ��룬
			��������ģ��ֵ��
		������
			linguisticFile��linguistic file��
		"""
		#read
		self.lingTbl_ = lingTbl
		multiFeatclsFuzzySet = {}
		
		#calculte fuzzy value of all feature classes 
		for lingValName in self.lingTbl_.Data().keys():
			lingVal = self.lingTbl_.Data()[lingValName]
			print "Now Analyse " + lingValName + "..."
			fuzzySet = self._ComputeProximity(self.inFeatcls_, lingVal)
			multiFeatclsFuzzySet[lingValName] = fuzzySet
		self.multiFeatclsFuzzySet_ = multiFeatclsFuzzySet
		
		return
	
	def SaveToFeatureClass(self, field = "Fuzzy_Grade"):
		"""
		������
			���������õ�ģ��ֵ��д������ĵ�Ҫ�����С�
		������
			field����д���ֶ�������Ҫȷ������ڡ�
		"""
		for featclsName in self.multiFeatclsFuzzySet_.keys():
			featclsFuzzys = self.multiFeatclsFuzzySet_[featclsName]
			copiedFeatcls = self._CopyFeatureClass(featclsName)
			fuzzyVals = featclsFuzzys
			
			oidField = pGP.describe(self.inFeatcls_).OIDFieldName
			rows = pGP.UpdateCursor(copiedFeatcls)
			row = rows.next()
			
			while row:
				row.SetValue(field, fuzzyVals[row.GetValue(oidField)])
				rows.UpdateRow(row)
				row = rows.next()
			
			del row
			del rows
		return
	
	#named constant for xml tags.
	xmlRootTag = "AlternativeCriterionEvaluationTable"
	criteriaListTag = "CriterionList"
	criteriaTag = "Criterion"
	matrixTag = "Matrix"
	altTag = "Alternative"
	
	def SaveToXMLFile(self, xmlFileName):
		#��ʼ��dom������ڵ�
		impl = getDOMImplementation()
		dom = impl.createDocument(None, self.xmlRootTag, None)
		criteriaListNode = dom.createElement(self.criteriaListTag)
		matrixNode = dom.createElement(self.matrixTag)
		dom.documentElement.appendChild(criteriaListNode)
		dom.documentElement.appendChild(matrixNode)
		
		altNodeDict = {}
		
		#��ÿ��Ҫ�أ���criteria��
		for criterianName in self.multiFeatclsFuzzySet_.keys():
			featclsFuzzys = self.multiFeatclsFuzzySet_[criterianName]
			
			#дcriteria����
			criteriaNode = dom.createElement(self.criteriaTag)
			criteriaNode.appendChild(dom.createTextNode(criterianName))
			criteriaListNode.appendChild(criteriaNode)
			
			#дmatrix
			for altName in featclsFuzzys.keys():
				if altName not in altNodeDict:
					altNodeDict[altName] = dom.createElement(self.altTag)
					altNodeDict[altName].setAttribute("name", str(altName))
					matrixNode.appendChild(altNodeDict[altName])
					
				altNode = altNodeDict[altName]
				altCrtValNode = dom.createElement(self.criteriaTag)
				altCrtValNode.setAttribute("name", criterianName)
				
				altCrtValNode.appendChild(dom.createTextNode(str(featclsFuzzys[altName])))
				altNode.appendChild(altCrtValNode)
				
		f = open(xmlFileName, "w")
		dom.writexml(f, '', '\t', '\n')
		f.close()	
		
	def _CopyFeatureClass(self, lingVal):
		"""
		����������linguistic value����Ҫ���ࡣ
		������
			lingVal : LingValue��ʵ����
		����ֵ��
			��������Ҫ�������ơ�
		"""
		destFeatcls = self.inFeatcls_ + "_" + lingVal.FeatCls
		if pGP.Exist(destFeatcls):
			pGP.Delete_management(destFeatcls)
		pGP.Copy_management(self.inFeatcls_, destFeatcls, "")
		return destFeatcls
	
	def _ComputeGeoProximity(self, ptsFeatcls, lingVal):
		"""
		������
			���ڴӵ�����Ϣ����ȡ�ٽ���Ϣ���ٽ���Ϣ�ĵ�λ�����ݱ���������
		���������
			ptsFeatcls: ��Ҫ���������
			lingVal: lingValue��ʵ��
		����ֵ��
			{ptId : proximity_ptId, ...}
				# Assign the values of parameters
		"""
		
		# Convert linguistic features to points
		path, name = os.path.split(self.inFeatcls_)
		oFeatCls = os.path.join(path, lingVal.FeatCls)
		
		ptsX, ptsY = build_value_lists(ptsFeatcls)
		ptsNearDists = {}
		
		studyArea = []
		
		#��ȡ������Ϣ����Vector����Distance������Raster����Extract
		if lingVal.FeatType == "Vector":
			featCls = FeatureClass(oFeatCls)
			#��ÿ��Ҫ�ظ������е�����������Ϣ��
			for feat in featCls.Features():
				#��Ҫ��
				if feat.ShapeTypeName() == Shape.Point:
					shpPt = Point.FromFeature(feat)
					#��õ㵽��ľ��룬������ÿһ������������Ҫ��
					for ptKey in ptsX.keys():
						dist = ((ptsX[ptKey] - shpPt.X) ** 2 + (ptsY[ptKey] - shpPt.Y) ** 2) ** 0.5
						ptsNearDists[ptKey] = min(dist, ptsNearDists.get(ptKey, dist))
				#�ߺ���Ҫ��
				else:
					shpLine = Line.FromFeature(feat)
					#�ֳɶ��segment����ÿ��segment�����������롣
					for seg in shpLine.OuteriorSegments():
						studyArea = [(pt.X, pt.Y) for pt in seg]
					
						#����С��3����Ҫ������ת�Ժ����ʹ��nearst_point
						if len(studyArea) < 3:
							studyAreaRev = copy(studyArea)
							studyAreaRev.reverse()
							studyArea += studyAreaRev[1:]
						
						#����segment������������ȫ�����������
						segNearestDists = nearest_point(studyArea, ptsX, ptsY)[0]
						for ptKey in segNearestDists.keys():
							dist = segNearestDists[ptKey]
							ptsNearDists[ptKey] = min(dist, ptsNearDists.get(ptKey, dist))
							
		if lingVal.FeatType == "Raster":
			#��ȡ��Ϣ����ʱҪ������
			tmpExtractedFeatcls = os.path.join(path, 'tmp_'+ name)
			rasterFeatCls = os.path.join(os.path.dirname(path), lingVal.FeatCls)
			
			logger.WriteLog("Extracting Values To Points...")
			pGP.CheckOutExtension("Spatial")
			pGP.ExtractValuesToPoints_sa(ptsFeatcls, rasterFeatCls, tmpExtractedFeatcls, "INTERPOLATE")
			
			#��ȡ��ʱҪ������Ϣ
			logger.WriteLog("Read Raster Values...")
			tmpFeatcls = FeatureClass(tmpExtractedFeatcls)
			ptsNearDists = dict([(feat.ID(), feat["RASTERVALU"]) for feat in tmpFeatcls.Features()])
			
			if pGP.Exists(tmpExtractedFeatcls):
				pGP.Delete_management(tmpExtractedFeatcls)
		
		return ptsNearDists
	def _ComputeProximity(self, ptFeatcls, lingVal):
		return self._ComputeGeoProximity(ptFeatcls, lingVal)

class ArcGISFuzzyProximityBuilder(ArcGISGeoProximityBuilder):
	def __init__(self, inFeatcls, distAlgo = None):
		ArcGISGeoProximityBuilder.__init__(self, inFeatcls, distAlgo)
		
	def _ComputeFuzzy(self, ptsFeatcls, lingVal):
		"""
		������
			ģ��ֵ���㡣
			��lingVal�ṩһ��linguistic value��
			�÷���������lingVal��ѡ��Ҫ���࣬����ptsFeatcls�ϸ�������Ŀ��Ҫ����ľ��룬
			������ģ���ȡ�
		���������
			ptsFeatcls����Ҫ�������ơ�
			lingVal��LingValue��ʵ����
		����ֵ��
			�������е㵽һ��Ҫ�����ģ��ֵ��
			��ʽ��
				[fuzzy0, fuzzy1, ...]
				����fuzzynΪptIdΪn�ĵ��ģ��ֵ��
		"""
		# Assign the values of parameters
		retFuzzys = {}
		
		ptsNearDists = self._ComputeGeoProximity(ptsFeatcls, lingVal)
				
		# ʹ����ȡ�������ݼ���ģ��ֵ
		# TODO: ����ʹ��Decoratorģʽ
		ptIds = ptsNearDists.keys()
		
		for ptId in ptIds:
			dist = ptsNearDists[ptId]
			if self.distAlgo_ == "Distance":
				retFuzzys[ptId] = dist
			else:
				if (lingVal.FuzzyType == "Near"):
					fuzzyVal = near_membership_function(
							dist,
							lingVal.a1,
							lingVal.r1
							)
				else:
					fuzzyVal = close_to_membership_function(
							dist,
							lingVal.a1,
							lingVal.r1,
							lingVal.a2,
							lingVal.r2
							)
				retFuzzys[ptId] = fuzzyVal
		return retFuzzys
	
	def _ComputeProximity(self, ptsFeatcls, lingVal):
		return self._ComputeFuzzy(ptsFeatcls, lingVal)
	
class ArcGISArithmeticLevelProximityBuilder(ArcGISGeoProximityBuilder):
	def __init__(self, inFeatcls, distAlgo = None):
		ArcGISGeoProximityBuilder.__init__(self, inFeatcls, distAlgo)
	
	@staticmethod
	def minmax_reducer(minmax_val, val):
		if not minmax_val:
			return (val, val)
		if val < mimmax_val[0]:
			return (val, minmax_val[1])
		if mimax_val[1] < val:
			return (minmax_val[0], val)
		return minmax_val
		
	def _ComputeArithmeticLevel(self, ptsFeatcls, lingVal):
		geoProximities = self._ComputeGeoPriximity(self, ptsFeatcls, lingVal)
		min, max = reduce(minmax_reducer, geoProximities.values())
		
		