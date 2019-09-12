#include "EquiArray.h"
#include "EquiPrimitive.h"


EquiObject* E_ARRAY_size::operator() (EquiObject* in)
{
	if (in->getType() != E_VOID_TYPE)
		throwError("No input expected in array.size function.");

	int i = ths->maxIndex();

	EquiPrimitive<int>* n = new EquiPrimitive<int>;
	n->setData(i);
	return n;
}

