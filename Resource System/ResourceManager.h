/*
	��Դʵ�ʵĹ���ϵͳ�����ڹ�����Դ��
*/
#pragma once

#include <hash_map>
#include <windows.h>
#include "NullStorage.h"
#include "SimpleResPolicy.h"

template <typename, typename> class HResource;
using namespace stdext;

//���࣬Managerʵ�ʵĹ����ߣ�TΪ�������������Դ����
template<typename T>
class ResourceManager
{
	friend class HResource;
	
protected:
	virtual BOOL ReleaseResource(size_t handle) = 0;

public:
	virtual BOOL Initialize() = 0;
	virtual BOOL Update() = 0;

	ResourceManager(){};

	virtual ~ResourceManager(void){}
};

/*Manager��һ�������࣬�ṩһ����չ��Դ������ķ�ʽ��
	ģ�������
		T				��Դ����
		Storage			�洢���ԣ�Լ���μ�NullStorage.h
		ManagerPolicy	��Դ������ԣ�Լ���μ�SimpleResPolicy.h
		ThreadingModel	�߳�ģ�ͣ�����֧�ֶ��߳�
*/
template	<	
			typename T, 
			template <typename ResType, template <typename> class StorageAllocator> class Storage, 
			template <typename Storage, typename ResType, template <typename> class Allocator, typename ThreadingModel> 
				class ManagerPolicy,
			typename ThreadingModel = SingleThreadModel,
			template <class> class StorageAllocator = std::allocator,
			template <class> class ResourceAllocator = std::allocator
			>
class ResourceManagerImpl : public ResourceManager<T>
{
	//���Ը���һ�ֹ��������ȡ������������
	template <class Other>
	struct Rebind {
		typedef 
			ResourceManagerImpl<Other, Storage, ManagerPolicy, StorageAllocator, ResourceAllocator> 
			other;
	};

	typedef typename ThreadingModel::Volatile<int>::Type VolatileInt;
	typedef ManagerPolicy<Storage<T, StorageAllocator>, T, ResourceAllocator, ThreadingModel> ResPolicy;
	typename ThreadingModel::Mutex m_mutex;

	ResPolicy m_res;
	typedef typename ThreadingModel::Lock Lock;
	
protected:
	virtual BOOL ReleaseResource(size_t handle) {
		Lock lock(m_mutex);
		return m_res.ReleaseResource(handle);
	}

public:
	virtual BOOL Initialize() {
		Lock lock(m_mutex);
		return m_res.Initialize();
	}

	virtual BOOL Update() {
		Lock lock(m_mutex);
		return m_res.Update();
	}

	//�����ַ�����ΪID����һ����Դ�����ؾ�������֧����������
	HResource<T, ThreadingModel> GetResource(LPCTSTR resstr) {
		Lock lock(m_mutex);
		size_t h;
		VolatileInt* pRefCount;
		T* res = m_res.GetResource(resstr, &h, &pRefCount);
		if(pRefCount) {
			return HResource<T, ThreadingModel>(res, this, pRefCount, h, false);
		}
		return HResource<T, ThreadingModel>(res, this, new int(1), h, true);	
	}
	template <typename P1>
	HResource<T, ThreadingModel> GetResource(LPCTSTR resstr, const P1& param1) {
		Lock lock(m_mutex);
		size_t h;
		VolatileInt* pRefCount;
		T* res = m_res.GetResource(resstr, &h, &pRefCount, param1);
		if(pRefCount) {
			return HResource<T, ThreadingModel>(res, this, pRefCount, h, false);
		}
		return HResource<T, ThreadingModel>(res, this, new VolatileInt(1), h, true);	
	}
	template <typename P1, typename P2>
	HResource<T, ThreadingModel> GetResource(LPCTSTR resstr, const P1& param1, const P2& param2) {
		Lock lock(m_mutex);
		size_t h;
		VolatileInt* pRefCount;
		T* res = m_res.GetResource(resstr, &h, &pRefCount, param1, param2);
		if(pRefCount) {
			return HResource<T, ThreadingModel>(res, this, pRefCount, h, false);
		}
		return HResource<T, ThreadingModel>(res, this, new VolatileInt(1), h, true);	
	}

	void ReleaseHResource(HResource<T, ThreadingModel>& hres)
	{
		hres.~HResource();
		hres.m_handleInManager = 0;
		hres.m_isOwnRCMgrment = true;
		hres.m_pManager = NULL;
		hres.m_pRefCount = NULL;
		hres.m_pResource = NULL;
	}

	~ResourceManagerImpl(void) {
		Lock lock(m_mutex);
		m_res.Clear();
	}
};