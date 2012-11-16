#pragma once

//�궨�壬��ĳ����Դ���ж������Լ���Null�����ʱ�����Ҫʹ�øú�����
//������ӵ���������
#define DECLARE_NULLOBJ(className) static const className nullObject;

//������ӵ����namespace��
#define DECLARE_HAS_NULLOBJ(className) \
	class className;\
	template<>\
		class null_type_traits<className>\
	{\
	public:\
		static const bool isHasNullObject = true;\
	};\

//�ж��ն����Traits
template<class T>
class null_type_traits
{
public:
	static const bool isHasNullObject = false;
};

//��ȡ��ȷ�Ļ�ȡ�ն���ĺ���
template<class T, bool isHasNullObject = null_type_traits<T>::isHasNullObject>
class NullObject
{
public:
	static const T& GetNullObject();
};

template<class T>
class NullObject<T, true>
{
public:
	static const T& GetConstNullObject() {
		return T::nullObject;
	}
	static T& GetNullObject() {
		MessageBox(NULL, _T("��ͼ���ն�����Ϊһ���ǳ������÷��أ�"), _T("����"), MB_OK);
		__asm{ int 3}
		return const_cast<T&>(T::nullObject);
	}
};

template<class T>
class NullObject<T, false>
{
public:
	static const T& GetConstNullObject() {
		MessageBox(NULL, _T("�Ƿ������ÿն���"), _T("����"), MB_OK);
		__asm{ int 3}

		static int i = 0;
		return i;
	}

	static T& GetNullObject() {
		MessageBox(NULL, _T("��ͼ���ն�����Ϊһ���ǳ������÷��أ�"), _T("����"), MB_OK);
		__asm{ int 3}

		static int i = 0;
		return i;
	}
};