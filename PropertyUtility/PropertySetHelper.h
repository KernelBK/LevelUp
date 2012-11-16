#include "PropertySet.h"
#include <cassert>

//��ʹ����PropertySet�����ṩ��ȡPropertySet�ķ�����ע����ಢû���ṩ��PropertySet�ṩ͸�����ʵĽӿڣ������ṩ�˻�ȡPropertySet�ķ�ʽ��
class IPropertySet
{
public:
	virtual PropertySet* GetPropertySet() = 0;

	template <class T> void Set(const wstring& key, const T& t) { GetPropertySet()->Set(this, key, t); }
	template <class T> void Get(const wstring& key, T& t) { GetPropertySet()->Get(this, key, t);}
};

//����ʹ�á����ԡ��ĸ��Ӷȣ����ṩ����ĺ�����
#define BEGIN_PROPERTY(className) protected: void InitProperties(PropertySet* pPropset) {
#define BEGIN_PROPERTY_EX(className, baseName)  protected: void InitProperties(PropertySet* pPropset) {baseName::InitProperties(pPropset);
#define REG_PROP_VAR(propName, var) pPropset->RegisterProperty(this, _T(#propName), &var);
#define END_PROPERTY(className) }\
								public:\
								/*template <class T> void Set(const wstring& key, const T& t) { GetPropertySet()->Set(this, key, t); }\
								template <class T> void Get(const wstring& key, T& t) { GetPropertySet()->Get(this, key, t);}*/\
								PropertySet* GetPropertySet() {static PropertySet _prop_set; static bool _is_init = false; if(!_is_init) {InitProperties(&_prop_set); _is_init = true;} return &_prop_set;}\
								private:

/*********************************

Example

class ExampleClass : public IPropertySet
{
	int foo;
	
	BEGIN_PROPERTY(ExampleClass)
		REG_PROP_VAR(fooName, foo)
	END_PROPERTY()
} eg;

eg.Set("fooName") = 5; // is eq to eg.foo = 5;
**********************************/