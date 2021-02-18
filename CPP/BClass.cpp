#include "BClass.h"

BClass::BClass() : TestNamespace::TestClass(0)
{
}

void BClass::BMethod()
{
	protectedInteger = 10;
}
