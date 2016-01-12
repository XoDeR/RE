#include "Core/Strings/DynamicString.h"

namespace Rio
{
	char* DynamicStringFn::copyDynamicStringToCString(const DynamicString& s, Allocator& a)
	{
		const char* cStr = s.toCStr();
		const size_t l = s.getLength() + 1;
		char* out = (char*)a.allocate(l);
		memcpy(out, cStr, l);
		return out;
	}

	DynamicString DynamicStringFn::join(const Array<char*>& a, const DynamicString& sep)
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
}