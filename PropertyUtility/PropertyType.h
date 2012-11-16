#pragma once
/************************************
���ļ��ṩ�����Ե� ����/����ID/�������� �����֧�֡�
************************************/	

//auto type register: �ṩ�Զ�����������ע�Ṧ�ܣ����ǿ��ܻ��з��գ�Ӧ������ʹ��
#define AUTO_TYPE_REGISTER_ENABLED

#include <vector>
#include <cassert>
#if defined(AUTO_TYPE_REGISTER_ENABLED)
	#include <typeinfo.h>	
	#include <cstdlib>
#endif
	
using namespace std;


/*****************************
����˵��������ʶ�𲿷�����Ҫ�ĸ�������
*****************************/
class NullType{};
class UnknownType{};
typedef size_t TypeID;

//�˴�����ע������
#define DECL_TYPES() \
DeclTypeName<char>(_T("char"));\
DeclTypeName<unsigned char>(_T("uchar"));\
DeclTypeName<short>(_T("short"));\
DeclTypeName<unsigned short>(_T("ushort"));\
DeclTypeName<int>(_T("int32"));\
DeclTypeName<unsigned int>(_T("uint"));\
DeclTypeName<float>(_T("single"));\
DeclTypeName<double>(_T("double"));\
DeclTypeName<std::string>(_T("std::string"));\
DeclTypeName<std::wstring>(_T("std::wstring"));

template <class T> class traits_type_to_id
{
public:
	static TypeID id;
};
template<class T> TypeID traits_type_to_id<T>::id = 0;

class TypeInfoList
{
	std::vector<const TCHAR *> m_typeInfos;
	static TypeInfoList* m_instance;

	template <class T> void AutoRegType();
	template <class T> void DeclTypeName_Nocheck(const TCHAR* str);
	template <class T> void DeclTypeName(const TCHAR* str);
	void InitTypeInfoList();

public:
	TypeInfoList()
	{
		InitTypeInfoList();
		DECL_TYPES();
	}
	
	const TCHAR * GetTypeName(TypeID tid);
	template <class T> TypeID GetTypeID();
	static TypeInfoList& GetInstance();

};

#define TYPETOID(type) (TypeInfoList::GetInstance().GetTypeID<type>())
#define TYPEID2STRING(tid) (TypeInfoList::GetInstance().GetTypeName(tid))
/**************************/

/****************************
Class TypeInfoList Implementation
****************************/
template <class T> TypeID TypeInfoList::GetTypeID() 
{
#if defined(AUTO_TYPE_REGISTER_ENABLED)
	AutoRegType<T>();
#endif
	return traits_type_to_id<T>::id;
}

/*****
��ע������ǲ����Ǽ�⣬������һЩ�������͵�ע�Ṥ������Ϊ��Щ��������������Чֵ����˿��ܺ����͵��б�ֵ��ͻ��
*****/
template <class T> void TypeInfoList::DeclTypeName_Nocheck(const TCHAR* str)
{
	traits_type_to_id<T>::id = m_typeInfos.size();
	m_typeInfos.push_back(str);
}

/*****
ע�᳣������ʱʹ�õĺ����������͡����ơ�ID��Ӧ������
*****/
template <class T> void TypeInfoList::DeclTypeName(const TCHAR* str)
{
	if(traits_type_to_id<T>::id == 0) {
		m_typeInfos.push_back(str); 
		traits_type_to_id<T>::id = m_typeInfos.size() - 1;
	} else {
		assert(!"Type Register Again!");
	}
}

/*****
������ɶ��ͱ���Զ�ע��
*****/
template <class T> void TypeInfoList::AutoRegType()
{
	static TCHAR typeName[256];
	if(traits_type_to_id<T>::id == 0)
	{
		mbstowcs(typeName, typeid(T).name(), 256);
		m_typeInfos.push_back(typeName); 
		traits_type_to_id<T>::id = m_typeInfos.size() - 1;
	}
}