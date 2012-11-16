#!/usr/bin/env python
# -*- coding: cp936 -*-

import os, traceback#, MainDocument

from PyQt4 import QtCore, QtGui
from UI.MainWnd_ui import *
from xml.dom.minidom import *

from AHP.AHPNameOnlyTreeEditor import *
from AHP.AHPTreeEditor import *

from AHP.Logic.AHPTree import *
from AHP.Logic.SensitiveAnalyst.AltCrtMatrix import *

from LocationAltGenerator.LinguisticEditor import *
from LocationAltGenerator.ProximityBuilderImpl.LinguisticTable import *
from ProximityBuilderWindow import *
from BuildRegPgWindow import *
from ResultImportWindow import *
from SensitiveAnalystWindow import *
from BatchExpWindow import *
from NEGSimWindow import *

from PyExtLib import XMLTreeRenderer

class MainWindow(QtCore.QObject):
	def __init__(self, dataset):
		QtCore.QObject.__init__(self)
		
		self.projDir_ = None
		self.dataset_ = dataset
		
		self.mainWnd_ = QtGui.QMainWindow()
		self.ui_ = Ui_mainWindow()
		self.ui_.setupUi(self.mainWnd_)
		
		self.ahpTreeEditor_ = None
		self.linguisticEditor_ = None
		self.ahpTree_ = None
		self.linguisticTbl_ = None
		
		self._SetupSlots()
		self._SetupActionsEnabled()
		
	def ShowWidgets(self):
		self.mainWnd_.show()
		
	def _SetupSlots(self):
		#�ļ��˵���
		self.connect(self.ui_.actionNew, QtCore.SIGNAL("triggered(bool)"), self._NewProject)
		self.connect(self.ui_.actionOpen, QtCore.SIGNAL("triggered(bool)"), self._OpenProject)
		self.connect(self.ui_.actionSave, QtCore.SIGNAL("triggered(bool)"), self._SaveProject)
		self.connect(self.ui_.actionSaveAs, QtCore.SIGNAL("triggered(bool)"), self._SaveProjectAs)
		#Modeling�˵�
		self.connect(self.ui_.actionConCrtTrees, QtCore.SIGNAL("triggered(bool)"), self._OpenAHPNameOnlyTreeEditor)
		self.connect(self.ui_.actionSetLingVal, QtCore.SIGNAL("triggered(bool)"), self._OpenLinguisticFileEditor)
		self.connect(self.ui_.actionBldRegPg, QtCore.SIGNAL("triggered(bool)"), self._OpenBuildRegPgWnd)
		self.connect(self.ui_.actionCompCrtProx, QtCore.SIGNAL("triggered(bool)"), self._OpenProximityBuilderWnd)
		#Criteria Det�˵�
		self.connect(self.ui_.actionSensAnalyst, QtCore.SIGNAL("triggered(bool)"), self._OpenSensAnalyst)
		self.connect(self.ui_.actionWPMSensAnalyst, QtCore.SIGNAL("triggered(bool)"), self._OpenWPMSensAnalyst)
		self.connect(self.ui_.actionAHPMatCon, QtCore.SIGNAL("triggered(bool)"), self._OpenAHPTreeEditor)
		#Tools �˵�
		self.connect(self.ui_.actionImport, QtCore.SIGNAL("triggered(bool)"), self._OpenImport)
		self.connect(self.ui_.actionAutoExp, QtCore.SIGNAL("triggered(bool)"), self._AutoExp)
		self.connect(self.ui_.actionTreeRenderer, QtCore.SIGNAL("triggered(bool)"), self._RenderTree)
		#NEG Simulation �˵�
		self.connect(self.ui_.actionDistMatGen, QtCore.SIGNAL("triggered(bool)"), self._GenerateDistanceMatrix)
		self.connect(self.ui_.actionSimulation, QtCore.SIGNAL("triggered(bool)"), self._OpenSimulator)
		return
	
	def _SetupActionsEnabled(self):
		#���п�����Ч�İ�ť����ΪFalse
		#File
		self.ui_.actionSaveAs.setEnabled(False)
		self.ui_.actionSave.setEnabled(False)
		#Modeling�˵�
		self.ui_.actionConCrtTrees.setEnabled(False)
		self.ui_.actionSetLingVal.setEnabled(False)
		self.ui_.actionBldRegPg.setEnabled(False)
		self.ui_.actionCompCrtProx.setEnabled(False)
		#Crt Det�˵�
		self.ui_.actionAHPMatCon.setEnabled(False)
		self.ui_.actionSensAnalyst.setEnabled(False)
		#View
		self.ui_.actionImport.setEnabled(False)
		self.ui_.actionTreeRenderer.setEnabled(False)
		#NEG Simulation
		self.ui_.actionGenDistMat.setEnabled(False)
		self.ui_.actionSimulation.setEnabled(False)
		
		if self.projDir_:
			#File
			self.ui_.actionSave.setEnabled(True)
			self.ui_.actionSaveAs.setEnabled(True)
			
			self.ui_.actionConCrtTrees.setEnabled(True)
			self.ui_.actionSetLingVal.setEnabled(True)
			self.ui_.actionBldRegPg.setEnabled(True)
			self.ui_.actionCompCrtProx.setEnabled(True)
			
			self.ui_.actionAHPMatCon.setEnabled(True)
			self.ui_.actionSensAnalyst.setEnabled(True)
			
			self.ui_.actionImport.setEnabled(True)
			self.ui_.actionTreeRenderer.setEnabled(True)
			
			self.ui_.actionGenDistMat.setEnabled(True)
			self.ui_.actionSimulation.setEnabled(True)
		
		if self.ahpTreeEditor_:
			pass
		if self.dataset_:
			pass
			
	def _NewProject(self, b):
		dir = unicode(
			QtGui.QFileDialog.getExistingDirectory(None, "�½�����".decode('cp936'))
			)
		if not dir: return
		
		dirInfo = QtCore.QFileInfo(dir)
		if not dirInfo.isDir():
			QtGui.QMessageBox.information(
				None,
				"����".decode('cp936'),
				"����ѡ���Ŀ¼��Ч����ѡ���˸�Ŀ¼���뽫�����½���һ���յ���ЧĿ¼�С�".decode('cp936')
				)
			return
		
		qdir = QtCore.QDir(dir)
		if qdir.entryList().count() > 2: #.��..����entryʼ�մ���
			QtGui.QMessageBox.information(
				None,
				"����".decode('cp936'),
				"����ѡ���Ŀ¼�Ѵ����ļ����뽫�����½������ļ����С�".decode('cp936')
				)
			return
		
		self._NewProjectImpl(dir)
		self._OpenProjectImpl(dir)
		return
	
	#��Ŀ¼���½���Ҫ�ļ�
	def _NewProjectImpl(self, dir):
		#AHPTree��������AHP����ÿ������Ȩ��
		ahpTreeFileContent =\
			"""<feature global_weight="1" local_weight="1" name="Root" >
				<Matrix/>
			</feature>"""	
		f = open(os.path.join(dir , "AHP.xml"), "w")
		f.write(ahpTreeFileContent)
		f.close()
		
		#Criteria����ļ���������Criterion��ֵ
		criteriaFileContent = """<Criteria/>"""
		f = open(os.path.join(dir , "Criteria.xml"), "w")
		f.write(criteriaFileContent)
		f.close()
		
		#Linguistic�ļ�
		linguisticFileContent = "Root Vector Near 10000 50000"
		f = open(os.path.join(dir , "linguistic file.txt"), "w")
		f.write(linguisticFileContent)
		f.close()
		
		#Distance Matrix��������Location�ľ������
		pass
	
	def _OpenProject(self):
		dir = unicode(
			QtGui.QFileDialog.getExistingDirectory(None, "�򿪹���".decode('cp936'))
			)
		if not dir: return
		self._OpenProjectImpl(dir)

	def _OpenProjectImpl(self, dir):
		#��������״̬
		oldAhpTree = self.ahpTree_
		oldLingTbl = self.linguisticTbl_
		oldAhpTreeEditor = self.ahpTreeEditor_
		oldLingEditor = self.linguisticEditor_
		
		oldProjDir = self.projDir_
		
		try:
			dirInfo = QtCore.QFileInfo(dir)
			if not dirInfo.isDir():
				raise Exception
			qdir = QtCore.QDir(dir)
			
			self.ahpTree_ = AHPTree()
			self.linguisticTbl_ = LinguisticTable(os.path.join(dir, "Linguistic File.txt"))
			
			self.linguisticEditor_ = LinguisticEditor(self.linguisticTbl_)
			self.ahpTreeEditor_ = AHPTreeEditor(os.path.join(dir, "AHP.xml"))
			self.ahpTree_ = self.ahpTreeEditor_.tree_
			
			self.projDir_ = dir
			MainDocument.mainDoc.ProjDir = dir
			self._SetupActionsEnabled()
			
		except:
			traceback.print_exc()
			
			#��ʧ����ָ�����״̬
			self.ahpTree_ = oldAhpTree
			self.linguisticTbl_ = oldLingTbl
			self.ahpTreeEditor_ = oldAhpTreeEditor
			self.linguisticEditor_ = oldLingEditor
			
			self.projDir_ = oldProjDir
			MainDocument.mainDoc.ProjDir = oldProjDir
			
			QtGui.QMessageBox.information(
				None,
				"����".decode('cp936'),
				"����ѡ���Ŀ¼��Ч����ѡ��һ����Ч�Ĺ���Ŀ¼�򿪡�".decode('cp936')
				)
			
			self._SetupActionsEnabled()
			
	def _SaveProject(self, b):
		pass
	
	def _SaveProjectAs(self, b):
		pass
	
	def _OpenAHPNameOnlyTreeEditor(self, b):
		editor = AHPNameOnlyTreeEditor(os.path.join(self.projDir_, "AHP.xml"))
		editor.widget_.setWindowTitle("AHP Tree Editor")
		editor.ShowWidgetModal()
		
	def _OpenAHPTreeEditor(self, b):
		self.ahpTreeEditor_ = AHPTreeEditor(os.path.join(self.projDir_, "AHP.xml"))
		self.ahpTreeEditor_.widget_.setWindowTitle("AHP Matrix Editor")
		self.ahpTreeEditor_.ShowWidget()
	
	def _OpenLinguisticFileEditor(self, b):
		self.linguisticTbl_.Load(os.path.join(self.projDir_, "linguistic file.txt"))
		self.linguisticEditor_ = LinguisticEditor(self.linguisticTbl_)
		self.linguisticEditor_.dlg_.setWindowTitle("Linguistic Table Editor")
		self.linguisticEditor_.ShowWidget()
		
	def _OpenBuildRegPgWnd(self, b):
		blgRegPgWnd = BuildRegPgWindow(self.dataset_)
		blgRegPgWnd.dlg_.setWindowTitle("Spatial Regular Tessella")
		blgRegPgWnd.ShowWidgetModal()
		
	def _OpenProximityBuilderWnd(self, b):
		proxBuilderWnd = ProximityBuilderWindow(self.dataset_, os.path.join(self.projDir_, "linguistic file.txt"))
		proxBuilderWnd.dlg_.setWindowTitle("Build Proximity")
		proxBuilderWnd.ShowWidgetModal()
	
	def _OpenSensAnalyst(self, b):
		ahpFile = os.path.join(self.projDir_, "AlternativeCriteriaEvaluationTable.xml")
		criteriaFile = os.path.join(self.projDir_, "Criteria.xml")
		altCrtMat = AltCrtMatrix()
		try:
			altCrtMat.Load(ahpFile, criteriaFile)
			altCrtViewer = SensitiveAnalystWindow(altCrtMat, "WSM")
			altCrtViewer.dlg_.setWindowTitle("Overall Results of the Comparative Study")
			altCrtViewer.ShowWidgetModal()
		except:
			traceback.print_exc()
			QtGui.QMessageBox.information(
				self.mainWnd_,
				"����".decode('cp936'),
				"Alternative - Criteria �����ڼ���ʱ������������Criteria�ļ���AltCrt�ļ��Ƿ�ƥ�䡣".decode('cp936')
				)
		pass
	
	def _OpenWPMSensAnalyst(self, b):
		ahpFile = os.path.join(self.projDir_, "AlternativeCriteriaEvaluationTable.xml")
		criteriaFile = os.path.join(self.projDir_, "Criteria.xml")
		altCrtMat = AltCrtMatrix()
		try:
			altCrtMat.Load(ahpFile, criteriaFile)
			altCrtViewer = SensitiveAnalystWindow(altCrtMat, "WPM")
			altCrtViewer.dlg_.setWindowTitle("Overall Results of the Comparative Study")
			altCrtViewer.ShowWidgetModal()
		except:
			traceback.print_exc()
			QtGui.QMessageBox.information(
				self.mainWnd_,
				"����".decode('cp936'),
				"Alternative - Criteria �����ڼ���ʱ������������Criteria�ļ���AltCrt�ļ��Ƿ�ƥ�䡣".decode('cp936')
				)
		pass		
	def _OpenImport(self, b):
		importWnd = ResultImportWindow(self.dataset_)
		importWnd.ShowWidgetModal()
		
	def _AutoExp(self, b):
		autoExpWnd = BatchExpWindow()
		autoExpWnd.ShowWidgetModal()
		pass
	
	def _RenderTree(self, b):
		XMLTreeRenderer.RenderXMLTree(os.path.join(self.projDir_, "AHP.xml").encode("cp936"), os.path.join(self.projDir_, "AHP.png").encode("cp936"))
		pixmap = QtGui.QPixmap(os.path.join(self.projDir_, "AHP.png"))
		self.ui_.label.setPixmap(pixmap.scaled(self.ui_.label.width(), self.ui_.label.height(), QtCore.Qt.IgnoreAspectRatio, QtCore.Qt.SmoothTransformation))
		
	def _GenerateDistanceMatrix(self, b):
		pass
	
	def _OpenSimulator(self, b):
		ahpFile = os.path.join(self.projDir_, "AlternativeCriteriaEvaluationTable.xml")
		criteriaFile = os.path.join(self.projDir_, "Criteria.xml")
		altCrtMat = AltCrtMatrix()
		try:
			altCrtMat.Load(ahpFile, criteriaFile)
		except:
			QtGui.QMessageBox.information(
				self.mainWnd_,
				"����".decode('cp936'),
				"��ȡAlternative - Criterion����ʱ����������ȷ���Ƿ��Ѿ�������MCE��".decode('cp936')
				)
			
		simWnd = NEGSimWindow(altCrtMat, self.dataset_)
		simWnd.ShowWidgetModal()