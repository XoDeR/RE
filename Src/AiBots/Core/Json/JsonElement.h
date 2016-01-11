// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Base/Types.h"

#include "Core/Strings/StringId32.h"
#include "Core/Strings/StringId64.h"
#include "Core/Strings/DynamicString.h"
#include "Core/Containers/Vector.h"

#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix4x4.h"

namespace Rio
{
	class JsonParser;
	class File;

	// Represents a JSON element.
	// The objects of this class are valid until the parser exists
	class JsonElement
	{
	public:
		// Construct the nil JsonElement.
		// Used to forward-instantiate elements or as a special nil element.
		JsonElement();
		explicit JsonElement(const char* at);
		JsonElement(const JsonElement& other);
		JsonElement& operator=(const JsonElement& other);
		// Returns the i-th item of the current array.
		JsonElement operator[](size_t i);
		JsonElement getElementByIndex(size_t i);
		// Returns the i-th item of the current array or
		// the special nil JsonElement() if the index does not exist.
		JsonElement getElementByIndexOrNil(size_t i);
		// Returns the element corresponding to key of the current object.
		// If the key is not unique in the object scope, the last key in order of appearance will be selected.
		JsonElement getValueByKey(const char* k);
		// Returns the element corresponding to key of the current object, or nil if the key does not exist.
		JsonElement tryGetValueByKey(const char* k);
		// Returns whether the element has the key.
		bool hasKey(const char* k) const;
		// Returns true whether the element is the JSON nil special value.
		bool isNil() const;
		// Returns true whether the element is a JSON boolean (true or false).
		bool isBool() const;
		// Returns true whether the element is a JSON number.
		bool isNumber() const;
		// Returns true whether the element is a JSON string.
		bool isString() const;
		// Returns true whether the element is a JSON array.
		bool isArray() const;
		// Returns true whether the element is a JSON object.
		bool isObject() const;
		// Returns the size of the element based on the element's type:
		// nil, bool, number: 1
		// string: length of the string
		// array: number of elements in the array
		// object: number of keys in the object
		size_t getJsonElementSize() const;
		bool toBool(bool def = false) const;
		int32_t toInt(int32_t def = 0) const;
		uint32_t toUint(uint32_t def = 0) const;
		float toFloat(float def = 0) const;
		void toString(DynamicString& str, const char* def = "") const;

		Vector2 toVector2(const Vector2& def = Vector2(0, 0)) const;
		Vector3 toVector3(const Vector3& def = Vector3(0, 0, 0)) const;
		Vector4 toVector4(const Vector4& def = Vector4(0, 0, 0, 0)) const;
		// Returns the Quaternion value of the element.
		Quaternion toQuaternion(const Quaternion& def = Quaternion::Identity) const;
		// Returns the Matrix4x4 value of the element.
		// Matrix4x4 = [x, x, x, x, y, y, y, y, z, z, z, z, t, t, t, t]
		Matrix4x4 toMatrix4x4(const Matrix4x4& def = Matrix4x4::Identity) const;
		// Returns the string id value hashed to murmur32() of the element.
		StringId32 toStringId32(const StringId32 def = StringId32(uint32_t(0))) const;
		// Returns the resource id value of the element.
		ResourceId toResourceId() const;
		// Returns the array value of the element.
		// Calling this function is faster than accessing individual
		// array elements by JsonElement::operator[] and it is the preferred way
		// for retrieving array elements. You should ensure that the array
		// contains only items of the given array type.
		void toArray(Array<bool>& array) const;
		void toArray(Array<int16_t>& array) const;
		void toArray(Array<uint16_t>& array) const;
		void toArray(Array<int32_t>& array) const;
		void toArray(Array<uint32_t>& array) const;
		void toArray(Array<float>& array) const;
		void toArray(Vector<DynamicString>& array) const;
		// Returns all the keys of the element.
		void getAllKeys(Vector<DynamicString>& keys) const;
	private:
		const char* jsonCurrentPos;
		friend class JsonParser;
	};
} // namespace Rio
