#include "TestClass.h"

TestNamespace::TestClass::TestClass(int i)
{
	pNumber = new int(i);
}

TestNamespace::TestClass::TestClass(TestClass& tc)
{
	pNumber = new int(*tc.pNumber);
}

TestNamespace::TestClass::~TestClass()
{
	if (pNumber != nullptr)
	{
		delete pNumber;
		pNumber = nullptr;
	}
}

void TestNamespace::TestClass::TestMethod()
{
}

void TestNamespace::TestClass::StaticTestMethod()
{
}
