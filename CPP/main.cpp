#include <Windows.h>
#include <iostream>
#include <vector>
#include <list>
#include "TestClass.h"
#include "BClass.h"
#include "StructsAndEnums.h"
#include "ImplementedClass.h"

using namespace std;
using namespace TestNamespace;

void TestFunc();
void Change(int i);
void ChangeWithPointer(int* pI);
void ChangeWithReference(int& i);
void PrintMyStructSize(MyStruct s, MyStruct* pS);
void CreateInteger(int** ppI);
void PrintArray(int arrLength, int* array);
inline int Min(int a, int b);

template<typename T>
T Max(T a, T b)
{
    return a > b ? a : b;
}

template<typename T>
void Bla(T a)
{
    a.Blub();
}

int main()
{
    cout << "Hello World!\n";

    TestClass c(0);
    c.TestMethod();
    TestNamespace::TestClass::StaticTestMethod();

    BClass b;
    b.TestMethod();

    TestFunc(); 

    MyStruct s;
    s.Number = 10;

    MyEnum e = MyEnum::Monday;
    s.Number = e;

    bool bl = 10; // true
    bl = 0; // false

    if (s.Number) {}
    if (s.Number != 0) {}

    auto var = 10; // like var in C#

    switch (var)
    {
    case 10:
        cout << 10 << endl;
    case 9:
        cout << 9 << endl;
    case 8:
        cout << 8 << endl;
    case 7:
        cout << 7 << endl;
    case 6:
        cout << 6 << endl;
    case 5:
        cout << 5 << endl;
    case 4:
        cout << 4 << endl;
    case 3:
        cout << 3 << endl;
    case 2:
        cout << 2 << endl;
    case 1:
        cout << 1 << endl;
    case 0:
        cout << 0 << endl;
    }

    // --------------------------------------------------------------------

    // Pointers
    int* pI = nullptr;
    int i = 22;
    pI = &i;

    cout << "pI(" << &pI << "): " << pI << " -> " << *pI << endl;

    int* pI2 = new int();
    delete pI2;
    pI2 = nullptr;

    int* pI3 = new int();
    pI3 = nullptr; // creating a memory leak

    int nr = 0;
    cout << "nr (" << &nr << "): " << nr << endl;
    Change(nr);
    cout << "nr (" << &nr << "): " << nr << endl;
    ChangeWithPointer(&nr);
    ChangeWithPointer(&nr);
    cout << "nr (" << &nr << "): " << nr << endl;
    ChangeWithReference(nr);
    ChangeWithReference(nr);
    cout << "nr (" << &nr << "): " << nr << endl;

    cout << sizeof(MyStruct) << endl;
    cout << sizeof(MyStruct*) << endl;

    PrintMyStructSize(s, &s);

    IInterface* pObj = nullptr;
    pObj = new ImplementedClass();
    (*pObj).FirstMethod(); 
    pObj->FirstMethod(); // both lines are equal
    //delete pObj; // raise an exception -> investigate this
    //pObj = nullptr;

    TestClass tc(0);
    *tc.pNumber = 10;
    cout << "tc (" << &tc << ")" << endl;
    cout << "tc.pNumber (" << tc.pNumber << "): " << *tc.pNumber << endl;
    TestClass tc2(tc);
    cout << "tc2 (" << &tc2 << ")" << endl;
    cout << "tc2.pNumber (" << tc2.pNumber << "): " << *tc2.pNumber << endl;

    // Pointer Pointers
    int* pSomething = nullptr;
    CreateInteger(&pSomething);

    cout << "pSomething: " << pSomething << endl;
    delete pSomething;
    pSomething = nullptr;

    // --------------------------------------------------------------------------

    // Arrays
	int array[]{ 0,1,2,3,4,5,6,7,8,9 };
    cout << array[0] << " " << array[1] << endl;
    int length = sizeof(array) / sizeof(int); // only possible if array is created in same scope
    for (int i = 0; i < length; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;

    PrintArray(length, array);

    const char* sz = "Test";
    const char sz2[5] = "Test"; // last character is \0

    int* dynArray = new int[] { 0, 1, 2, 3, 4};
    delete[] dynArray;
    dynArray = nullptr;

    //int* pointerArray[]{ new int(), new int(), new int() };
    //int** dynPointerArray = new int*[]{ new int(), new int(), new int() };

    cout << *(array + 1) << endl; // BE CAREFUL!!!

    cout << Min(1, 3) << endl; // cout << 1 < 3 ? 1 : 3 << endl;
    cout << Min(7, 2) << endl; // cout << 7 < 2 ? 7 : 2 << endl;
    cout << Min(nr, i) << endl;

    ImplementedClass* pImplClass = dynamic_cast<ImplementedClass*>(pObj);
    ImplementedClass* pImplClass2 = static_cast<ImplementedClass*>(pObj); // be careful, because no runtime check
    //int* pSomethingElse = reinterpret_cast<int*>(pObj); // HELP!!! ERROR!!! BE CAREFUL!!!
    char* sz3 = const_cast<char*>(sz);
    const int cInt = 10;
    int integer = cInt;

    Max(1, 2);
    Max(1.0f, 2.0f);
    Max(string{ "Test" }, string{"Test2"});
    //Max(tc, tc2); // because > is not defined for TestClass

    //Bla(tc); // because Blub() is not defined for TestClass

    return 0;
}

void PrintArray(int arrLength, int* array)
{
    cout << "sizeof array: " << sizeof(array) << endl;
    for (int i = 0; i < arrLength; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;
}

inline int Min(int a, int b)
{
    return a < b ? a : b;
}

void CreateInteger(int** ppI)
{
    *ppI = new int();
}

void TestFunc()
{

}

void Change(int i)
{
    i += 10;
    cout << "Change: " << &i << endl;
}

void ChangeWithPointer(int* pI)
{
    *pI += 10;
    cout << "ChangeWithPointer: " << &pI << " " << pI << endl;
}

void ChangeWithReference(int& i)
{
    i += 10;
    cout << "ChangeWithReference: " << &i << endl;
}

void PrintMyStructSize(MyStruct s, MyStruct* pS)
{
    cout << sizeof(s) << endl;
    cout << sizeof(pS) << endl;
}