#include "StringId.h"


const StringId kInvalidStringId;

///////////////////////////////////////////////////////////////
// Copy and Move operators.
///////////////////////////////////////////////////////////////;
StringId& StringId::operator=(const StringId& right)
{
	if (this == &right)
		return *this;

	Assign(right);
	return(*this);
}

StringId& StringId::operator=(StringId&& right)
{
	if (this == &right)
		return *this;

	Assign(right);
	return(*this);
};
///////////////////////////////////////////////////////////////
// Searches of the Id of ths string, if not found a new ID is created and inserted into the static map. 
///////////////////////////////////////////////////////////////
void StringId::FindOrCreateStringId(const std::string& str)
{
	const StringIdMap::iterator findIt = s_stringIdMap.find(str);
	if (findIt != s_stringIdMap.end())
		m_pData = &(*findIt);
	else if (str == kInvalidString)
		m_pData = &kInvalidString;
	else
		m_pData = &(*s_stringIdMap.emplace(str).first);
};

void StringId::FindOrCreateStringId(std::string&& str)
{
	const StringIdMap::iterator findIt = s_stringIdMap.find(str);
	if (findIt != s_stringIdMap.end())
		m_pData = &(*findIt);
	else if (str == kInvalidString)
		m_pData = &kInvalidString;
	else
		m_pData = &(*s_stringIdMap.emplace(std::move(str)).first);
};
