#pragma once

#include "tools/Utilities/StringId.h"

using ResourceId = StringId;

///////////////////////////////////////////////////////////////
// Virtual class used with textures, audio, components, 
///////////////////////////////////////////////////////////////
class Resource
{
	ResourceId m_resourceId;
public:
	virtual ~Resource() = default;
	virtual bool Load(const StringId& id) = 0;
};