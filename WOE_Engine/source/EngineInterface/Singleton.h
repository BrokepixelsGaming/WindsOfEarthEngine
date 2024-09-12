#pragma once

#include <assert.h>
#include "tools/BleachNew.h"

///////////////////////////////////////////////////////////////
// Template class used to with things that will only ever be one. 
///////////////////////////////////////////////////////////////
template<class Type>
class Singleton
{

public:
		
	static void Create() { assert(s_pType == nullptr); s_pType = BLEACH_NEW(Type); }
	static void Destroy() { assert(s_pType); BLEACH_DELETE(s_pType); };
	static Type* Get() { return s_pType; }

private:
	inline static Type* s_pType = nullptr ;
protected:
	Singleton(){};
};