#pragma once
class AbstractClass
{
public:
	virtual void PureVirtualMethod() = 0;

	inline int GetNumberExplicit(); // explicit inline
	int GetNumberImplicit() { return _number; } // implicit inline

protected:
	int _number = 0;
};

