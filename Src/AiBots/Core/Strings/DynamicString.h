// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Debug/Error.h"
#include "Core/Memory/Memory.h"
#include "Core/Strings/StringUtils.h"
#include "Core/Containers/Array.h"
#include "Core/Strings/StringUtils.h"
#include "Core/Strings/StringId32.h"

#include <algorithm>
#include <cstring>

namespace Rio 
{

// dynamic strings
// Array is used as internal container

class DynamicString
{
public:
	DynamicString(Allocator& allocator = getDefaultAllocator());
	DynamicString(const char* s, Allocator& allocator = getDefaultAllocator());
	DynamicString(char c, Allocator& a = getDefaultAllocator());
	DynamicString(const char* s, size_t length, Allocator& a = getDefaultAllocator());
	DynamicString(const DynamicString& s);

	~DynamicString() = default;

	DynamicString& operator+=(const DynamicString& s);
	DynamicString& operator+=(const char* s);
	DynamicString& operator+=(const char c);

	DynamicString& operator=(const DynamicString& s);
	DynamicString& operator=(const char* s);
	DynamicString& operator=(const char c);

	bool operator<(const DynamicString& s) const;

	inline bool operator<(const char* rhs)
	{
		return strCmp(this->toCStr(), rhs) < 0;
	}

	inline bool operator<=(const DynamicString& rhs)
	{
		return strCmp(this->toCStr(), rhs.toCStr()) <= 0;
	}

	inline bool operator<=(const char* rhs)
	{
		return strCmp(this->toCStr(), rhs) <= 0;
	}

	inline bool operator>(const DynamicString& rhs)
	{
		return strCmp(this->toCStr(), rhs.toCStr()) > 0;
	}

	inline bool operator>(const char* rhs)
	{
		return strCmp(this->toCStr(), rhs) > 0;
	}

	inline bool operator>=(const DynamicString& rhs)
	{
		return strCmp(this->toCStr(), rhs.toCStr()) >= 0;
	}

	inline bool operator>=(const char* rhs)
	{
		return strCmp(this->toCStr(), rhs) >= 0;
	}

	bool operator==(const DynamicString& s) const;
	bool operator==(const char* s) const;
	inline bool operator!=(const DynamicString& rhs)
	{
		return !operator==(rhs);
	}

	inline bool operator!=(const char* rhs)
	{
		return !operator==(rhs);
	}

	inline char& operator[](size_t index) 
	{ 
		return stringData[index]; 
	}
	inline const char& operator[](size_t index) const 
	{ 
		return stringData[index]; 
	}

	size_t append(char c);
	size_t append(const char* cStr);
	size_t append(const DynamicString& other);

	size_t getLength() const;
	size_t getCapacity() const;

	// Removes the leading string s.
	// The string must start with s.
	void stripLeading(const char* s);

	// Removes the trailing string s.
	// The string must end with s.
	void stripTrailing(const char* s);
	// Returns whether the string starts with the given s string.
	bool startsWith(const char* s) const;
	bool endsWith(const char* s) const;
	StringId32 toStringId32() const;
	const char* toCStr() const;
	inline char* begin() 
	{ 
		return ArrayFn::begin(stringData); 
	}
	inline char* end() 
	{ 
		return ArrayFn::end(stringData); 
	}
	inline char& front() 
	{ 
		return ArrayFn::front(stringData); 
	}
	inline char& back() 
	{ 
		return ArrayFn::back(stringData); 
	}
	inline void clear() 
	{ 
		ArrayFn::clear(stringData); 
	}
	inline void shrinkToFit() 
	{ 
		ArrayFn::condense(stringData); 
		if (back() != '\0')
		{
			ArrayFn::pushBack(stringData, '\0');
		}
	}

	inline void resize(size_t length) 
	{ 
		ArrayFn::resize(stringData, length); 
		ArrayFn::condense(stringData);
		if (back() != '\0')
		{
			ArrayFn::pushBack(stringData, '\0');
		}
	}

	inline void reserve(size_t capacity) 
	{ 
		ArrayFn::reserve(stringData, capacity); 
	}

	inline DynamicString substring(size_t begin, size_t end)
	{
		if (end > this->getLength())
		{
			end = this->getLength();
		}

		DynamicString out(*(stringData.allocator));
		size_t l = end - begin;
		out.reserve(l);

		for (size_t i = begin; i < end; i++)
		{
			out.append(this[i]);
		}
		return out;
	}

	// returns the index of the first instance of sep in s, or -1 if sep is not present
	size_t findFirst(const DynamicString& sep);
	// returns the index of the first instance of any character from
	// chars in the string, or -1 if no character from chars is present
	size_t findFirstFromSet(const DynamicString& chars);
	// counts the number of non-overlapping instances of sep
	size_t getOccurencesCount(const DynamicString& sep);
	// returns the index of the last instance of sep in s, or -1 if sep is not present in the string
	size_t findLast(const DynamicString& sep);
	// returns the index of the last instance of any character from
	// chars in the string, or -1 if no character from chars is present
	size_t findLastFromSet(const DynamicString& chars);
	// returns a new string consisting of count copies of the string
	DynamicString getRepeated(size_t count);
	// returns true if substr is within
	bool contains(const DynamicString& substr);
	// returns true if any character in chars is within
	bool containsAny(const DynamicString& chars);
	// tests whether the string begins with prefix
	bool hasPrefix(const DynamicString& prefix);
	// tests whether the string ends with suffix
	bool hasSuffix(const DynamicString& suffix);
	DynamicString toLower();
	DynamicString toTitle();
	DynamicString toUpper();
	DynamicString trimLeft(const DynamicString& cutset);
	DynamicString trimRight(const DynamicString& cutset);
	// trim(s, cutset) is equivalent to trimLeft(trimRight(s, cutset), cutset)
	DynamicString trim(const DynamicString& cutset);
	DynamicString trimSpace();
	static char* copyDynamicStringToCString(const DynamicString& s, Allocator& a);
	// splits the string into all substrings separated by sep and sets the out
	// array to the substrings between those separators
	void split(const DynamicString& sep, Array<char*>& out);
	// concatenates the elements of a to create a single string
	// The separator string sep is placed between elements in the resulting string
	static DynamicString join(const Array<char*>& array, const DynamicString& sep);
private:
	// how string is represented:
	// "abc"
	// [0] == "a"; [1] == "b"; [2] == "c"; [3] == "\0"
	Array<char> stringData;
};

inline DynamicString::DynamicString(Allocator& allocator)
	: stringData(allocator)
{
	ArrayFn::pushBack(stringData, '\0');
}

inline DynamicString::DynamicString(const char* s, Allocator& allocator)
	: stringData(allocator)
{
	if (s != nullptr)
	{
		ArrayFn::push(stringData, s, strLen(s));
	}
	ArrayFn::pushBack(stringData, '\0');
}

inline DynamicString::DynamicString(char c, Allocator& allocator)
	: stringData(allocator)
{
	append(c);
}

inline DynamicString::DynamicString(const char* s, size_t length, Allocator& allocator)
	: stringData(allocator)
{
	ArrayFn::push(stringData, s, length);
	ArrayFn::pushBack(stringData, '\0');
}

inline DynamicString::DynamicString(const DynamicString& s)
	: stringData(s.stringData)
{
}

inline DynamicString& DynamicString::operator+=(const DynamicString& s)
{
	return *this += s.toCStr();
}

inline DynamicString& DynamicString::operator+=(const char* s)
{
	RIO_ASSERT_NOT_NULL(s);
	ArrayFn::popBack(stringData);
	ArrayFn::push(stringData, s, strLen(s));
	ArrayFn::pushBack(stringData, '\0');
	return *this;
}

inline DynamicString& DynamicString::operator+=(const char c)
{
	ArrayFn::popBack(stringData);
	ArrayFn::pushBack(stringData, c);
	ArrayFn::pushBack(stringData, '\0');
	return *this;
}

inline DynamicString& DynamicString::operator=(const DynamicString& s)
{
	stringData = s.stringData;
	return *this;
}

inline DynamicString& DynamicString::operator=(const char* s)
{
	RIO_ASSERT_NOT_NULL(s);
	ArrayFn::clear(stringData);
	ArrayFn::push(stringData, s, strLen(s));
	ArrayFn::pushBack(stringData, '\0');
	return *this;
}

inline DynamicString& DynamicString::operator=(const char c)
{
	ArrayFn::clear(stringData);
	ArrayFn::pushBack(stringData, c);

	return *this;
}

inline bool DynamicString::operator<(const DynamicString& s) const
{
	return strCmp(this->toCStr(), s.toCStr()) < 0;
}

inline bool DynamicString::operator==(const DynamicString& s) const
{
	return strCmp(this->toCStr(), s.toCStr()) == 0;
}

inline bool DynamicString::operator==(const char* s) const
{
	RIO_ASSERT_NOT_NULL(s);
	return strCmp(this->toCStr(), s) == 0;
}

inline size_t DynamicString::append(char c)
{ 
	ArrayFn::popBack(this->stringData);
	ArrayFn::pushBack(this->stringData, c);
	ArrayFn::pushBack(this->stringData, '\0');
	return getLength();
}

inline size_t DynamicString::append(const char* cStr)
{
	ArrayFn::popBack(this->stringData);
	ArrayFn::push(this->stringData, cStr, strLen(cStr));
	ArrayFn::pushBack(stringData, '\0');
	return getLength();
}

inline size_t DynamicString::append(const DynamicString& other)
{
	ArrayFn::popBack(this->stringData);
	ArrayFn::push(this->stringData, &other.stringData[0], other.getLength());
	ArrayFn::pushBack(stringData, '\0');
	return getLength();
}

inline size_t DynamicString::getLength() const
{
	return strLen(this->toCStr());
}

inline size_t DynamicString::getCapacity() const
{
	return ArrayFn::getCapacity(stringData);
}

inline void DynamicString::stripLeading(const char* s)
{
	RIO_ASSERT_NOT_NULL(s);
	RIO_ASSERT(startsWith(s), "String does not start with %s", s);

	const size_t thisLength = strLen(this->toCStr());
	const size_t sLength = strLen(s);

	memmove(ArrayFn::begin(stringData), ArrayFn::begin(stringData) + sLength, (thisLength - sLength));
	ArrayFn::resize(stringData, thisLength - sLength);
	ArrayFn::pushBack(stringData, '\0');
}

inline void DynamicString::stripTrailing(const char* s)
{
	RIO_ASSERT_NOT_NULL(s);
	RIO_ASSERT(endsWith(s), "String does not end with %s", s);

	const size_t thisLength = strLen(this->toCStr());
	const size_t sLength = strLen(s);
	ArrayFn::resize(stringData, thisLength - sLength);
	ArrayFn::pushBack(stringData, '\0');
}

inline bool DynamicString::startsWith(const char* s) const
{
	RIO_ASSERT_NOT_NULL(s);
	return strnCmp(this->toCStr(), s, strLen(s)) == 0;
}

inline bool DynamicString::endsWith(const char* s) const
{
	RIO_ASSERT_NOT_NULL(s);

	const size_t thisLength = strLen(this->toCStr());
	const size_t sLength = strLen(s);

	if (thisLength >= sLength)
	{
		return strnCmp(ArrayFn::begin(stringData) + (thisLength - sLength), s, sLength) == 0;
	}

	return false;
}

inline StringId32 DynamicString::toStringId32() const
{
	return StringId32(this->toCStr());
}

inline const char* DynamicString::toCStr() const
{
	return ArrayFn::begin(stringData);
}

inline size_t DynamicString::findFirst(const DynamicString& sep)
{
	const size_t n = sep.getLength();
	const size_t sl = this->getLength();

	if (n == 0)
		return 0;
	if (n == 1)
	{
		const char c = sep[0];
		for (size_t i = 0; i < sl; i++)
		{
			if ((*this)[i] == c)
				return (int64_t)i;
		}
		return -1;
	}
	if (n == sl)
	{
		if (sep == *this)
			return 0;
		return -1;
	}
	if (n > sl)
		return -1;

	// Search using Rabin-Karp
	uint32_t hashsep, power;
	hashStr(sep, hashsep, power);
	uint32_t h = 0;
	for (size_t i = 0; i < n; i++)
	{
		h = h * PrimeRK + (uint32_t)(*this)[i];
	}
	if (h == hashsep && this->substring(0, n) == sep)
	{
		return 0;
	}
	for (size_t i = n; i < sl;)
	{
		h *= PrimeRK;
		h += (uint32_t)(*this)[i];
		h -= power * (uint32_t)(*this)[i - n];
		i++;
		if (h == hashsep && this->substring(i - n, i) == sep)
			return i - n;
	}

	return -1;
}

inline size_t DynamicString::findFirstFromSet(const DynamicString& chars)
{
	const size_t cLen = chars.getLength();
	const size_t sLen = this->getLength();

	if (cLen > 0)
	{
		for (size_t i = 0; i < sLen; i++)
		{
			for (size_t j = 0; j < cLen; j++)
			{
				if ((*this)[i] == chars[j])
					return (int64_t)i;
			}
		}
	}

	return -1;
}

inline size_t DynamicString::getOccurencesCount(const DynamicString& sep)
{
	size_t n = 0;

	const size_t ls = this->getLength();
	const size_t lsep = sep.getLength();

	if (lsep == 0)
		return ls + 1;
	if (lsep == 1)
	{
		char c = sep[0];
		for (size_t i = 0; i < ls; i++)
		{
			if ((*this)[i] == c)
				n++;
		}

		return n;
	}
	if (lsep > ls)
		return 0;
	if (lsep == ls)
	{
		if (sep == *this)
			return 1;
		return 0;
	}

	// Rabin-Karp search
	uint32_t hashsep, power;
	hashStr(sep, hashsep, power);

	uint32_t h = 0;
	for (size_t i = 0; i < lsep; i++)
		h = h * PrimeRK + (uint32_t)(*this)[i];

	size_t lastMatch = 0;
	DynamicString sepMod = sep; // a copy
	if (h == hashsep && sep == sepMod.substring(0, lsep))
	{
		n++;
		lastMatch = lsep;
	}

	for (size_t i = lsep; i < ls; /**/)
	{
		h *= PrimeRK;
		h += (uint32_t)(*this)[i];
		h -= power * (uint32_t)(*this)[i - lsep];
		i++;
		if (h == hashsep && lastMatch <= i - lsep && 
			substring(i - lsep, i) == sep)
		{
			n++;
			lastMatch = i;
		}
	}

	return n;
}

inline size_t DynamicString::findLast(const DynamicString& sep)
{
	size_t n = sep.getLength();

	if (n == 0)
		return this->getLength();
	if (n == 1)
	{
		char c = sep[0];
		for (size_t i = this->getLength() -1; i >= 0; i--)
		{
			if ((*this)[i] == c)
				return i;
		}
		return -1;
	}
	if (n == this->getLength())
	{
		if (sep == *this)
			return 0;
		return -1;
	}
	if (n > this->getLength())
		return -1;

	uint32_t hashsep, power;
	hashStrRev(sep, hashsep, power);
	size_t last = this->getLength() - n;

	uint32_t h = 0;
	for (size_t i = this->getLength() - 1; i >= last; i--)
		h = h * PrimeRK + (uint32_t)(*this)[i];

	if (h == hashsep && substring(last, this->getLength()) == sep)
		return last;

	for (size_t i = last - 1; i >= 0; i--)
	{
		h *= PrimeRK;
		h += (uint32_t)(*this)[i];
		h -= power * (uint32_t)(*this)[i + n];
		if (h == hashsep && substring(i, i + n) == sep)
			return i;
	}

	return -1;
}

inline size_t DynamicString::findLastFromSet(const DynamicString& chars)
{
	if (chars.getLength() > 0)
	{
		for (size_t i = this->getLength() -1; i >= 0; i--)
		{
			for (size_t j = 0; j < chars.getLength(); j++)
			{
				if ((*this)[i] == chars[j])
					return i;
			}
		}
	}

	return -1;
}

inline DynamicString DynamicString::getRepeated(size_t count)
{
	DynamicString out;
	out.reserve(count * this->getLength());

	for (size_t i = 0; i < count; i++)
		out.append(*this);

	return out;
}

inline bool DynamicString::contains(const DynamicString& substr)
{
	return findFirst(substr) >= 0;
}

inline bool DynamicString::containsAny(const DynamicString& chars)
{
	return findFirstFromSet(chars) >= 0;
}

inline bool DynamicString::hasPrefix(const DynamicString& prefix)
{
	return this->getLength() >= prefix.getLength() && substring(0, prefix.getLength()) == prefix;
}

inline bool DynamicString::hasSuffix(const DynamicString& suffix)
{
	return this->getLength() >= suffix.getLength() &&
		substring(this->getLength() - suffix.getLength(), this->getLength()) == suffix;
}

inline DynamicString DynamicString::toLower()
{
	DynamicString out = *this;

	const size_t ls = this->getLength();
	for (size_t i = 0; i < ls; i++)
		out[i] = Rio::toLower((*this)[i]);

	return out;
}

inline DynamicString DynamicString::toTitle()
{
	const size_t ls = this->getLength();
	if (ls == 0)
		return *this;

	DynamicString out = *this;

	size_t i = 0;
	while (Rio::isSpace(out[i]))
		i++;

	out[i] = Rio::toUpper(out[i]);
	i++;

	for (; i < ls; i++)
	{
		// If the previous character is a space, then capitalize the current one
		if (Rio::isSpace(out[i - 1]))
			out[i] = Rio::toUpper(out[i]);
	}

	return out;
}

inline DynamicString DynamicString::toUpper()
{
	DynamicString out = *this;

	const size_t ls = this->getLength();
	for (size_t i = 0; i < ls; i++)
		out[i] = Rio::toUpper((*this)[i]);

	return out;
}

inline DynamicString DynamicString::trimLeft(const DynamicString& cutset)
{
	const size_t slen = this->getLength();
	const size_t clen = cutset.getLength();
	if (this->getLength() == 0)
		return *this;
	int64_t pos = -1;
	bool t = false;

	for (size_t i = 0; i < slen; i++)
	{
		for (size_t j = 0; j < clen; j++)
		{
			if ((*this)[i] == cutset[j])
			{
				t = true;
				pos = i;
			}
		}
		if (!t)
			break;
		t = false;
	}

	if (pos != -1)
		return substring(pos + 1, slen);
	return *this;
}

inline DynamicString DynamicString::trimRight(const DynamicString& cutset)
{
	const size_t slen = this->getLength();
	const size_t clen = cutset.getLength();
	if (slen == 0)
		return *this;
	int64_t pos = -1;
	bool t = false;

	for (size_t i = slen - 1; i >= 0; i--)
	{
		for (size_t j = 0; j < clen; j++)
		{
			if ((*this)[i] == cutset[j])
			{
				t = true;
				pos = i;
			}
		}
		if (!t)
			break;
		t = false;
	}

	if (pos != -1)
		return substring(0, pos);
	return *this;
}

inline DynamicString DynamicString::trim(const DynamicString& cutset)
{
	return trimRight(cutset).trimLeft(cutset);
}

inline DynamicString DynamicString::trimSpace()
{
	return trim(" \t\n\v\f\r");
}

static char* copyDynamicStringToCString(const DynamicString& s, Allocator& a)
{
	const char* cStr = s.toCStr();
	const size_t l = s.getLength() + 1;
	char* out = (char*)a.allocate(l);
	memcpy(out, cStr, l);
	return out;
}

void DynamicString::split(const DynamicString& sep, Array<char*>& out)
{
	Allocator& a = *(out.allocator);

	if (sep == "")
	{
		const size_t n = this->getLength();

		ArrayFn::resize(out, n);

		for (size_t i = 0; i < n; i++)
			out[i] = copyDynamicStringToCString(s[i], a);

		return;
	}

	const size_t n = getOccurencesCount(sep) + 1;
	const char c = sep[0];

	size_t start = 0;
	size_t na = 0;
	ArrayFn::resize(out, n);

	const size_t lsep = sep.getLength();
	const size_t ls = this->getLength();

	for (size_t i = 0; i + lsep <= ls && na + 1 < n; i++)
	{
		if (s[i] == c && (lsep == 1 || substring(i, i + lsep) == sep))
		{
			out[na] = copyDynamicStringToCString(substring(start, i), a);
			na++;
			start = i + lsep;
			i += lsep - 1;
		}
	}

	out[na] = copyDynamicStringToCString(substring(start, ls), a);

	ArrayFn::resize(out, na + 1);
}

DynamicString DynamicString::join(const Array<char*>& a, const DynamicString& sep)
{
	const int64_t aLen = ArrayFn::getCount(a);
	if (aLen == 0)
		return "";

	if (aLen == 1)
		return a[0];

	int64_t n = sep.getLength() * (aLen - 1);
	for (int64_t i = 0; i < aLen; i++)
		n += Rio::strLen(a[i]);

	DynamicString out = a[0];

	for (int64_t i = 1; i < aLen; i++)
	{
		out.append(sep);
		out.append(a[i]);
	}

	return out;
}

// Aux functions
namespace
{
	const static uint32_t PrimeRK = 16777619;
} // namespace (anonymous)

// returns the hash and the appropriate multiplicative factor for use in the Rabin-Karp algorithm
static void hashStr(const DynamicString& sep, uint32_t& hash, uint32_t& power)
{
	hash = 0;

	const size_t lsep = sep.getLength();
	for (size_t i = 0; i < lsep; i++)
		hash = hash * PrimeRK + (uint32_t)sep[i];

	power = 1;

	uint32_t sq = PrimeRK;
	for (size_t i = lsep; i > 0; i >>= 1)
	{
		if ((i & 1) != 0)
			power *= sq;
		sq *= sq;
	}
}

// returns the hash of the reverse of sep and the appropriate
// multiplicative factor for use in Rabin-Karp algorithm.
static void hashStrRev(const DynamicString& sep, uint32_t& hash, uint32_t& power)
{
	hash = 0;

	const size_t lsep = sep.getLength();
	for (size_t i = lsep - 1; i >= 0; i--)
		hash = hash * PrimeRK + (uint32_t)sep[i];

	power = 1;

	uint32_t sq = PrimeRK;
	for (size_t i = lsep; i > 0; i >>= 1)
	{
		if ((i & 1) != 0)
			power *= sq;
		sq *= sq;
	}
}

} // namespace Rio
