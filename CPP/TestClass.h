#pragma once
#ifndef _TESTCLAS_H_
#define _TESTCLAS_H_

//#ifdef _DEBUG
namespace TestNamespace
{
	class TestClass
	{
	public:
		TestClass(int i); // constructor
		TestClass(TestClass& tc); // copy constructor
		~TestClass(); // destructor
		void TestMethod();
		void static StaticTestMethod();

		int* pNumber = nullptr;

	protected:
		int protectedInteger;
	};
}
//#endif // _DEBUG

#endif // !_TESTCLAS_H_