/*
	֧�����ü���
*/
#pragma once
#include <string>
#include "NullClassTraits.h"
#include "ThreadModel.h"

using namespace std;

//��Դ����࣬ע�⣬��һ����ֻ���Ա���һ�̷߳��ʣ��������߳�ͬʱ���ʸö�����Ҫcopy
template<typename T, typename ThreadingModel = SingleThreadModel>
class HResource
{
	friend class ResourceManagerImpl;

protected:
	typedef typename ThreadingModel::Volatile<int>::Type VolatileInt;
	bool				m_isOwnRCMgrment;
	size_t				m_handleInManager;
	VolatileInt*		m_pRefCount;
	ResourceManager<T>* m_pManager;
	T*					m_pResource;

	
	/*
	���캯��������ResourceManager���Ե��á�
		�������壺
		pResource	��Դָ��
		pManager	������ָ��
		pRefCount	�洢���ü�����ָ��
		h			���������ɾ���á�
		isMgrment	ָʾ���ɹ������������ü���ָ�뻹���ɾ���������ü���ָ��
	*/
	HResource(T* pResource, ResourceManager<T>* pManager, VolatileInt* pRefCount, size_t h, bool isMgrment)
		:	m_pManager(pManager), 
			m_pResource(pResource),
			m_pRefCount(pRefCount), 
			m_handleInManager(h), 
			m_isOwnRCMgrment(isMgrment)
	{}

public:
	static BOOL IsNullHandle(HResource& hres) {
		return m_pResource != NULL;
	}

	ResourceManager<T>* GetManager(){
		return m_pManager;
	}

	HResource(const HResource& rhs)
		:	m_pRefCount(rhs.m_pRefCount), m_pResource(rhs.m_pResource),
			m_pManager(rhs.m_pManager), m_handleInManager(rhs.m_handleInManager),
			m_isOwnRCMgrment(rhs.m_isOwnRCMgrment)
	{
		if(m_pRefCount){
			ThreadingModel::Inc(m_pRefCount);
		}
	}

	HResource& operator = (const HResource& rhs) {	
		if(this == &rhs) return *this;
		this->~HResource();
		m_pRefCount = rhs.m_pRefCount;
		if(m_pRefCount){
			ThreadingModel::Inc(m_pRefCount);
		}
		m_pManager = rhs.m_pManager;
		m_pResource = rhs.m_pResource;
		m_handleInManager = rhs.m_handleInManager,
		m_isOwnRCMgrment = rhs.m_isOwnRCMgrment;
		return *this;
	}

	const T& operator* () const{
		return (m_pResource ? *m_pResource : NullObject<T>::GetConstNullObject());
	}

	T& operator* () {
		return (m_pResource ? *m_pResource : NullObject<T>::GetNullObject());
	}

	~HResource(void) {
		if(!m_pResource) return;
		if(ThreadingModel::Dec(m_pRefCount) == 0) {
			m_pManager->ReleaseResource(m_handleInManager);
			if(m_isOwnRCMgrment) {
				delete m_pRefCount;
			}
		}
	}
};