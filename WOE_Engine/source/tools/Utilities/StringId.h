#pragma once

#include <string>
#include <unordered_set>
#include <assert.h>

class StringId;

using StringIdMap = std::unordered_set<std::string>;
extern const StringId kInvalidStringId;


class StringId
{
	friend bool operator<(const StringId& left, const StringId& right);

	inline static const std::string kInvalidString = "kInvalidStringId"; // string for the Id is invalid.
	inline static StringIdMap s_stringIdMap;

	const std::string* m_pData; // Always points to a vaild string. 

public:
	// Construction
	StringId() : m_pData(&kInvalidString) {}
	StringId(const std::string& str) { FindOrCreateStringId(str); }
	StringId(const char* pStr) { FindOrCreateStringId(pStr); }
	StringId(const StringId& strId) { Assign(strId); }
	StringId(std::string&& str) { FindOrCreateStringId(std::move(str)); }

	// Accessors
	const std::string& Get() const { assert(m_pData); return (*m_pData); }
	StringId& operator=(const StringId& right);
	StringId& operator=(StringId&& right);
	StringId& operator=(const std::string& right) { FindOrCreateStringId(right); return (*this); }
	StringId& operator=(std::string&& str) { FindOrCreateStringId(std::move(str)); return (*this); }
	StringId& operator=(const char* right) { FindOrCreateStringId(right); return (*this); }
	StringId& operator+=(const std::string& right) { FindOrCreateStringId((*m_pData) + right); return (*this); }

	// Comparison operators
	bool IsValid() const { return(m_pData != nullptr && m_pData != kInvalidStringId.m_pData); }
	bool operator==(const char* right) const { return (std::strcmp(m_pData->c_str(), right) == 0); }
	bool operator!=(const char* right) const { return (std::strcmp(m_pData->c_str(), right) != 0); }
	bool operator==(const std::string& right) const { return (*m_pData == right); }
	bool operator!=(const std::string& right) const { return (*m_pData != right); }
	bool operator==(const StringId& right) const { return m_pData == right.m_pData; }
	bool operator!=(const StringId& right) const { return m_pData != right.m_pData; }

	// Conversion operators
	operator const char* () const { assert(m_pData); return (m_pData->c_str()); }
	operator const std::string& () const { assert(m_pData); return (*m_pData); }

private:
	void FindOrCreateStringId(const std::string& str);
	void FindOrCreateStringId(std::string&& str);
	void Assign(const StringId& strId) { m_pData = strId.m_pData; }

};

///////////////////////////////////////////////////////////////
// Global < operator required for STL map/set insertions.
// IMPORTANT StringId < StringId is not the same as std::string < std::string!
///////////////////////////////////////////////////////////////
inline bool operator<(const StringId& left, const StringId& right)
{
	return (left.m_pData < right.m_pData);
};

///////////////////////////////////////////////////////////////
// Hasing functor for StringId's. use this functor whenever you want to use a StringId as a key in a hash table. 
///////////////////////////////////////////////////////////////
class StringIdStdHashFunctor
{
public:
	size_t operator()(const StringId& key) const
	{
		const char* val = key.Get().c_str();
		std::hash<const char*> hashFunc;
		return hashFunc(val);
	}
};