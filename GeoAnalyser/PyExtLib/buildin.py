# -*- coding: cp936 -*-

class sdict:
	"""һ������һ�µ��ֵ䣬key��list��ʽ�洢����֤˳��"""
	def __init__(self, lst = None):
		try:
			self.dict_ = dict(lst)
			self.keyList_ = zip(*lst)[0]
		except:
			self.dict_ = {}
			self.keyList_ = []
	
	#emulating build-in functions
	def __getitem__(self, key):
		return self.dict_[key]
	
	def __setitem__(self, key, value):
		if not self.dict_.has_key(key):
			self.keyList_.append(key)
		self.dict_[key] = value
	
	def __delitem__(self, key):
		del self.dict_[key]
		self.keyList_.remove(key)
	
	def __iter__(self):
		return self.iterkeys()
	
	def __contains__(self, item):
		return item in self.dict_
	
	#emulating dictionary
	def keys(self):
		return [key for key in self.keyList_]
	
	def values(self):
		return [self.dict_[key] for key in self.keyList_]
	
	def items(self):
		return [(key, self.dict_[key]) for key in self.keyList]
	
	def has_key(self, key):
		return self.dict_.has_key(key)
	
	def get(self, *args, **keywords):
		return self.dict_.get(*args, **keywords)
	
	def clear(self):
		self.dict_.clear()
		self.keyList_.clear()
	
	def setdefault(self, k, x = None):
		if k not in self.dict_:
			self.keyList_.append(k)
		return self.dict_.setdefault(k, x)
	
	def iterkeys(self):
		return iter(self.keys())
	
	def itervalues(self):
		return iter(self.values())
	
	def iteritems(self):
		return iter(self.items())
	
	def pop(self, x = None):
		"""������в�Ϊ���򵯳��������򷵻�x"""
		if len(self.keyList_) == 0:
			return x
		key = self.keyList_.pop()
		return self.dict_.pop(key, x)
	
	def popitem(self):
		#��ջ��ֱ�ӵ������쳣
		if len(self.keyList_) == 0:
			return self.dict_.popitem()
		#�ǿ�����ȷ��ջ��֤��ȷ��
		key = self.keyList_.pop()
		return (key, self.dict_.pop(key, x))
	
	def copy(self):
		retSDict = sdict()
		retSDict.keyList_ = [key for key in self.keyList_]
		retSDict.dict_ = self.dict_.copy()
	
	def update(self, kvPairList):
		for kvPair in kvPairList:
			self.__setitem__(kvPair[0], kvPair[1])
	
	#emulating mutable sequences
	def append(self, kvPair):
		"""׷�ӡ����key�Ѵ��������׷��"""
		if kvPair[0] not in self.dict_:
			self.__setitem__(kvPair[0], kvPair[1])
	
	def count(self):
		return len(self.keyList_)
	
	def index(self, key):
		return self.keyList_.index(key)
	
	def extend(self, kvPairList):
		for kvPair in kvPairList:
			self.append(kvPair)
	
	def insert(self, i, kvPair):
		if not self.dict_.has_key(kvPair[0]):
			self.keyList_.insert(i, kvPair[0])
			self.dict_[kvPair[0]] = kvPair[1]
	
	def reverse(self):
		self.keyList_.reserve()
	
	def sort(self, *args, **keywords):
		self.keyList_.sort(*args, **keywords)
	
	#other useful functions
	def setifexists(self, key, value):
		if self.has_key(key):
			self[key] = value
			