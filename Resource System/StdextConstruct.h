#pragma once
#include <xmemory>

_STD_BEGIN

/*
ִ��ʵ�ʵ�Construct�ĺ������Ƕ�Std::_Construct����չ
*/
template <class T> inline 
void _Construct(T* p) {
	new ((void*)p) T;
}
template <class T, class P1, class P2>
inline void _Construct(T* p, const P1& p1, const P2& p2)
{
	new ((void*)p) T(p1, p2);
}

/*
��ִ���κβ�����Construct
*/
template <typename T> inline
void _ConstructNon(T* p) {
}
template <typename T, typename P1> inline
void _ConstructNon(T* p, const P1& p1) {
}
template <typename T, typename P1, typename P2> inline
void _ConstructNon(T* p, const P1& p1, const P2& p2) {
}

/*
������ԣ����ݷ�����������ȷ��ʹ�����ֹ��췽ʽ�����÷�������Construct����ʹ�����õ�Construct
*/
template <class T>
struct ConstructTraits {
	static const bool IsDoConstructInAllocator = false;
};

/*
�����࣬���ڽ���ʵ�ʵĹ��칤�����������ѡ��
*/
template <class Allocator, class T, bool B = ConstructTraits<T>::IsDoConstructInAllocator>
struct Construct {
};

template <class Allocator, class T>
struct Construct<Allocator, T, true> {
	template <typename T>
	void operator ()(Allocator& al, T* p) {
		al.construct(p);
	}
	template <typename T, typename P1>
	void operator ()(Allocator& al, T* p, const P1& p1) {
		al.construct(p, p1);
	}
	template <typename T, typename P1, typename P2>
	void operator ()(Allocator& al, T* p, const P1& p1, const P2& p2) {
		al.construct(p, p1, p2);
	}
};

template <class Allocator, class T>
struct Construct<Allocator, T, false> {
	template <typename T>
	void operator ()(Allocator& al, T* p) {
		_Construct(p);
	}
	template <typename T, typename P1>
	void operator ()(Allocator& al, T* p, const P1& p1) {
		_Construct(p, p1);
	}
	template <typename T, typename P1, typename P2>
	void operator ()(Allocator& al, T* p, const P1& p1, const P2& p2) {
		_Construct(p, p1, p2);
	}
};
_STD_END

/*
֧�ֺ꣬���ڸ��߱����������λ��
*/
#define DECLARE_CONSTRUCT_IN_ALLOCATOR(className) \
	template<>\
	class ConstructTraits<className>\
	{\
		static const bool IsConstructInAllocator = true;\
	}