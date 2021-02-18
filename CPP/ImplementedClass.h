#pragma once
#include "AbstractClass.h"
#include "IInterface.h"

class ImplementedClass :
    public AbstractClass, public IInterface
{
public:
    void PureVirtualMethod() override;
    void FirstMethod() override;
    void SecondMethod() override;
};

