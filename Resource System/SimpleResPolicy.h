#pragma once
#include <windows.h>
#include "MemAllocator.h"
/*
ģ�������
	StoragePolicy	�洢����
	ResType			��Դ����
	Allocator		��Դ�ķ�����
	ThreadingModel	�߳�ģ��

����û���ṩ�̰߳�ȫ�����������ĵ����߱�֤����Ϊ�̰߳�ȫ�ṩ���Ͷ���֧�֡�
*/
template 
<
	typename StoragePolicy,
	typename ResType,
	template <typename> class Allocator,
	class ThreadingModel
>
class SimpleResPolicy
{
protected:
	//��һ������Ҫ����ʵ�ʵĴ洢�ṹ
	typedef typename ThreadingModel::Volatile<int>::Type VolatileInt;
	typedef VolatileInt Storage;

	Storage	m_storage;
	//�������빹����
	Allocator<ResType> m_resAllocator;
	Construct<Allocator<ResType>, ResType> m_construct;
	//����Ϊ����ʵ�ֵĺ���

public:
	//��ʼ�����������Storage����һЩ��ʼ������
	BOOL Initialize() {
		m_storage = 0;
		return TRUE;
	}

	//���£���Storage���и��²���
	BOOL Update() {
		return TRUE;
	}

	/*��ȡ��Դ������ָ����Դ���͵�ָ��
	������
		[in ]resstr		��Դ�ַ���������ָʾ��Դ����������Ψһ
		[out]h			��������������ڲ��Ҷ�Ӧ��ֵ�ã�������HResource<T>�У�����ΪTCHAR*
		[out]ppRefCount	���ü�����ָ���ָ�룬���ڷ������ü�����ָ�룬��HResource<T>�������ü����ã������Դ�������û�б����ָ�룬��Ӧ������NULL
	*/
	ResType* GetResource(LPCTSTR resstr, size_t* h, VolatileInt** ppRefCount) {
		ThreadingModel::Inc(&m_storage);
		*ppRefCount = NULL;
		ResType* p = m_resAllocator.allocate(1);
		*h = (size_t)((void*)p);
		m_construct(m_resAllocator, p);
		return p;		
	}
	template <typename P1>
	ResType* GetResource(LPCTSTR resstr, size_t* h, VolatileInt** ppRefCount, const P1& param1) {
		ThreadingModel::Inc(&m_storage);
		*ppRefCount = NULL;
		ResType* p = m_resAllocator.allocate(1);
		*h = (size_t)((void*)p);
		m_construct(m_resAllocator, p, p1, p2);
		return p;		
	}
	template <typename P1, typename P2>
	ResType* GetResource(LPCTSTR resstr, size_t* h, VolatileInt** ppRefCount, const P1& param1, const P2& param2)
	{
		ThreadingModel::Inc(&m_storage);
		*ppRefCount = NULL;
		ResType* p = m_resAllocator.allocate(1);
		*h = (size_t)((void*)p);
		m_construct(m_resAllocator, p, p1, p2);
		return p;		
	}

	BOOL ReleaseResource(size_t h) {
		if(h != 0) {
			ThreadingModel::Dec(&m_storage);
			m_resAllocator.destroy((ResType*)h);
			m_resAllocator.deallocate((ResType*)h, 1);
		}
		return TRUE;
	}
	void Clear() {
		if(m_storage) {
			__asm { int 3 }
		}
	}
};

