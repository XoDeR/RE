// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Json/JsonElement.h"

#include "Core/Json/JsonParser.h"
#include "Core/Json/Rjson.h"
#include "Core/Memory/TempAllocator.h"
#include "Core/Strings/StringUtils.h"
#include "Core/Containers/Map.h"
#include "Core/FileSystem/File.h"

namespace Rio
{
	JsonElement::JsonElement()
		: jsonCurrentPos(NULL)
	{
	}

	JsonElement::JsonElement(const char* at)
		: jsonCurrentPos(at)
	{
	}

	JsonElement::JsonElement(const JsonElement& other)
		: jsonCurrentPos(other.jsonCurrentPos)
	{
	}

	JsonElement& JsonElement::operator=(const JsonElement& other)
	{
		// begin is the other's current position
		jsonCurrentPos = other.jsonCurrentPos;
		return *this;
	}

	JsonElement JsonElement::operator[](size_t i)
	{
		Array<const char*> array(getDefaultAllocator());
		Rjson::parseArray(jsonCurrentPos, array);
		RIO_ASSERT(i < ArrayFn::getCount(array), "Index out of bounds");
		return JsonElement(array[i]);
	}

	JsonElement JsonElement::getElementByIndex(size_t i)
	{
		return this->operator[](i);
	}

	JsonElement JsonElement::getElementByIndexOrNil(size_t i)
	{
		if (jsonCurrentPos != NULL)
		{
			Array<const char*> array(getDefaultAllocator());
			Rjson::parseArray(jsonCurrentPos, array);
			if (i >= ArrayFn::getCount(array))
			{
				return JsonElement();
			}
			return JsonElement(array[i]);
		}
		return JsonElement();
	}

	JsonElement JsonElement::getValueByKey(const char* k)
	{
		Map<DynamicString, const char*> object(getDefaultAllocator());
		Rjson::parseObject(jsonCurrentPos, object);

		const char* value = MapFn::get(object, DynamicString(k), (const char*)NULL);
		RIO_ASSERT(value != NULL, "Key not found: '%s'", k);

		return JsonElement(value);
	}

	JsonElement JsonElement::tryGetValueByKey(const char* k)
	{
		if (jsonCurrentPos != nullptr)
		{
			Map<DynamicString, const char*> object(getDefaultAllocator());
			Rjson::parseObject(jsonCurrentPos, object);

			const char* value = MapFn::get(object, DynamicString(k), (const char*)NULL);

			if (value != nullptr)
			{
				return JsonElement(value);
			}
		}
		return JsonElement();
	}

	bool JsonElement::hasKey(const char* k) const
	{
		Map<DynamicString, const char*> object(getDefaultAllocator());
		Rjson::parseObject(jsonCurrentPos, object);
		return MapFn::has(object, DynamicString(k));
	}

	bool JsonElement::toBool(bool def) const
	{
		return isNil() ? def : Rjson::parseBool(jsonCurrentPos);
	}

	int32_t JsonElement::toInt(int32_t def) const
	{
		return isNil() ? def : Rjson::parseInt(jsonCurrentPos);
	}

	uint32_t JsonElement::toUint(uint32_t def) const
	{
		return isNil() ? def : Rjson::parseUint(jsonCurrentPos);
	}

	float JsonElement::toFloat(float def) const
	{
		return isNil() ? def : Rjson::parseFloat(jsonCurrentPos);
	}

	void JsonElement::toString(DynamicString& str, const char* def) const
	{
		if (isNil())
		{
			str = def;
		}
		else
		{
			Rjson::parseString(jsonCurrentPos, str);
		}
	}

	Vector2 JsonElement::toVector2(const Vector2& def) const
	{
		if (isNil())
			return def;

		TempAllocator64 alloc;
		Array<const char*> array(alloc);
		Rjson::parseArray(jsonCurrentPos, array);

		return Vector2(Rjson::parseFloat(array[0]), Rjson::parseFloat(array[1]));
	}

	Vector3 JsonElement::toVector3(const Vector3& def) const
	{
		if (isNil())
			return def;

		TempAllocator64 alloc;
		Array<const char*> array(alloc);
		Rjson::parseArray(jsonCurrentPos, array);

		return Vector3(Rjson::parseFloat(array[0]),
			Rjson::parseFloat(array[1]),
			Rjson::parseFloat(array[2]));
	}

	Vector4 JsonElement::toVector4(const Vector4& def) const
	{
		if (isNil())
			return def;

		TempAllocator64 alloc;
		Array<const char*> array(alloc);
		Rjson::parseArray(jsonCurrentPos, array);

		return Vector4(Rjson::parseFloat(array[0]),
			Rjson::parseFloat(array[1]),
			Rjson::parseFloat(array[2]),
			Rjson::parseFloat(array[3]));
	}

	Quaternion JsonElement::toQuaternion(const Quaternion& def) const
	{
		if (isNil())
			return def;

		TempAllocator64 alloc;
		Array<const char*> array(alloc);
		Rjson::parseArray(jsonCurrentPos, array);

		const Vector3 axis = Vector3(Rjson::parseFloat(array[0])
			, Rjson::parseFloat(array[1])
			, Rjson::parseFloat(array[2])
			);
		const float angle = Rjson::parseFloat(array[3]);

		return Quaternion(axis, angle);
	}

	Matrix4x4 JsonElement::toMatrix4x4(const Matrix4x4& def) const
	{
		if (isNil())
			return def;

		TempAllocator128 alloc;
		Array<float> array(alloc);
		toArray(array);

		return Matrix4x4(ArrayFn::begin(array));
	}

	StringId32 JsonElement::toStringId32(const StringId32 def) const
	{
		if (isNil())
			return def;

		TempAllocator1024 alloc;
		DynamicString str(alloc);
		Rjson::parseString(jsonCurrentPos, str);
		return str.toStringId32();
	}

	ResourceId JsonElement::toResourceId() const
	{
		DynamicString str(getDefaultAllocator());
		Rjson::parseString(jsonCurrentPos, str);
		return ResourceId(str.toCStr());
	}

	void JsonElement::toArray(Array<bool>& array) const
	{
		Array<const char*> temp(getDefaultAllocator());
		Rjson::parseArray(jsonCurrentPos, temp);
		for (size_t i = 0; i < ArrayFn::getCount(temp); i++)
		{
			ArrayFn::pushBack(array, Rjson::parseBool(temp[i]));
		}
	}

	void JsonElement::toArray(Array<int16_t>& array) const
	{
		Array<const char*> temp(getDefaultAllocator());
		Rjson::parseArray(jsonCurrentPos, temp);
		for (size_t i = 0; i < ArrayFn::getCount(temp); i++)
		{
			ArrayFn::pushBack(array, (int16_t)Rjson::parseInt(temp[i]));
		}
	}

	void JsonElement::toArray(Array<uint16_t>& array) const
	{
		Array<const char*> temp(getDefaultAllocator());
		Rjson::parseArray(jsonCurrentPos, temp);
		for (size_t i = 0; i < ArrayFn::getCount(temp); i++)
		{
			ArrayFn::pushBack(array, (uint16_t)Rjson::parseInt(temp[i]));
		}
	}

	void JsonElement::toArray(Array<int32_t>& array) const
	{
		Array<const char*> temp(getDefaultAllocator());
		Rjson::parseArray(jsonCurrentPos, temp);
		for (size_t i = 0; i < ArrayFn::getCount(temp); i++)
		{
			ArrayFn::pushBack(array, (int32_t)Rjson::parseInt(temp[i]));
		}
	}

	void JsonElement::toArray(Array<uint32_t>& array) const
	{
		Array<const char*> temp(getDefaultAllocator());
		Rjson::parseArray(jsonCurrentPos, temp);
		for (size_t i = 0; i < ArrayFn::getCount(temp); i++)
		{
			ArrayFn::pushBack(array, (uint32_t)Rjson::parseInt(temp[i]));
		}
	}

	void JsonElement::toArray(Array<float>& array) const
	{
		Array<const char*> temp(getDefaultAllocator());
		Rjson::parseArray(jsonCurrentPos, temp);
		for (size_t i = 0; i < ArrayFn::getCount(temp); i++)
		{
			ArrayFn::pushBack(array, Rjson::parseFloat(temp[i]));
		}
	}

	void JsonElement::toArray(Vector<DynamicString>& array) const
	{
		Array<const char*> temp(getDefaultAllocator());
		Rjson::parseArray(jsonCurrentPos, temp);
		for (size_t i = 0; i < ArrayFn::getCount(temp); i++)
		{
			DynamicString str;
			Rjson::parseString(temp[i], str);
			VectorFn::pushBack(array, str);
		}
	}

	void JsonElement::getAllKeys(Vector<DynamicString>& keys) const
	{
		Map<DynamicString, const char*> object(getDefaultAllocator());
		Rjson::parseObject(jsonCurrentPos, object);

		const Map<DynamicString, const char*>::Node* it = MapFn::begin(object);
		while (it != MapFn::end(object))
		{
			VectorFn::pushBack(keys, (*it).pair.first);
			it++;
		}
	}

	bool JsonElement::isNil() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::NIL;
		}
		return true;
	}

	bool JsonElement::isBool() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::BOOL;
		}
		return false;
	}

	bool JsonElement::isNumber() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::NUMBER;
		}
		return false;
	}

	bool JsonElement::isString() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::STRING;
		}

		return false;
	}

	bool JsonElement::isArray() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::ARRAY;
		}
		return false;
	}

	bool JsonElement::isObject() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::OBJECT;
		}

		return false;
	}

	size_t JsonElement::getJsonElementSize() const
	{
		if (jsonCurrentPos == NULL)
		{
			return 0;
		}

		switch (Rjson::getJsonType(jsonCurrentPos))
		{
		case JsonValueType::NIL:
		{
			return 1;
		}
		case JsonValueType::OBJECT:
		{
			Map<DynamicString, const char*> object(getDefaultAllocator());
			Rjson::parseObject(jsonCurrentPos, object);
			return MapFn::getCount(object);
		}
		case JsonValueType::ARRAY:
		{
			Array<const char*> array(getDefaultAllocator());
			Rjson::parseArray(jsonCurrentPos, array);
			return ArrayFn::getCount(array);
		}
		case JsonValueType::STRING:
		{
			DynamicString string;
			Rjson::parseString(jsonCurrentPos, string);
			return string.getLength();
		}
		case JsonValueType::NUMBER:
		{
			return 1;
		}
		case JsonValueType::BOOL:
		{
			return 1;
		}
		default:
		{
			RIO_FATAL("Unknown JSON value type");
			return 0;
		}
		}
	}
} // namespace Rio
