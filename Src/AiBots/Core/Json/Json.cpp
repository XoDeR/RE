// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Json/Json.h"
#include "Core/Strings/StringUtils.h"
#include "Core/Memory/TempAllocator.h"
#include "Core/Containers/Map.h"

namespace Rio
{

	namespace Json
	{
		static const char* getNext(const char* json, const char c = 0)
		{
			RIO_ASSERT_NOT_NULL(json);

			if (c && c != *json)
			{
				RIO_ASSERT(false, "Expected '%c' got '%c'", c, *json);
			}

			return ++json;
		}

		static const char* skipString(const char* json)
		{
			RIO_ASSERT_NOT_NULL(json);

			bool escaped = false;

			while ((*(json = getNext(json))) != 0)
			{
				if (*json == '"' && !escaped)
				{
					json = getNext(json);
					return json;
				}
				else if (*json == '\\') escaped = true;
				else escaped = false;
			}

			return json;
		}

		static const char* skipValue(const char* json)
		{
			RIO_ASSERT_NOT_NULL(json);

			switch (*json)
			{
				case '"': json = skipString(json); break;
				case '[': json = skipBlock(json, '[', ']'); break;
				case '{': json = skipBlock(json, '{', '}'); break;
				default: for (; *json != ',' && *json != '}' && *json != ']'; ++json); break;
			}

			return json;
		}

		JSONValueType::Enum getJsonType(const char* json)
		{
			RIO_ASSERT_NOT_NULL(json);

			switch (*json)
			{
				case '"': return JSONValueType::STRING;
				case '{': return JSONValueType::OBJECT;
				case '[': return JSONValueType::ARRAY;
				case '-': return JSONValueType::NUMBER;
				default: return (isdigit(*json)) ? JSONValueType::NUMBER : (*json == 'n' ? JSONValueType::NIL : JSONValueType::BOOL);
			}
		}

		void parseString(const char* json, DynamicString& string)
		{
			RIO_ASSERT_NOT_NULL(json);

			if (*json == '"')
			{
				while (*(json = getNext(json)))
				{
					// Empty string
					if (*json == '"')
					{
						json = getNext(json);
						return;
					}
					else if (*json == '\\')
					{
						json = getNext(json);

						switch (*json)
						{
							case '"': string += '"'; break;
							case '\\': string += '\\'; break;
							case '/': string += '/'; break;
							case 'b': string += '\b'; break;
							case 'f': string += '\f'; break;
							case 'n': string += '\n'; break;
							case 'r': string += '\r'; break;
							case 't': string += '\t'; break;
							default:
							{
								RIO_FATAL("Bad escape character");
								break;
							}
						}
					}
					else
					{
						string += *json;
					}
				}
			}

			RIO_FATAL("Bad string");
		}

		double parseDouble(const char* json)
		{
			RIO_ASSERT_NOT_NULL(json);

			TempAllocator512 alloc;
			Array<char> number(alloc);

			if (*json == '-')
			{
				ArrayFn::pushBack(number, '-');
				json = getNext(json, '-');
			}
			while (isdigit(*json))
			{
				ArrayFn::pushBack(number, *json);
				json = getNext(json);
			}

			if (*json == '.')
			{
				ArrayFn::pushBack(number, '.');
				while ((*(json = getNext(json))) && isdigit(*json))
				{
					ArrayFn::pushBack(number, *json);
				}
			}

			if (*json == 'e' || *json == 'E')
			{
				ArrayFn::pushBack(number, *json);
				json = getNext(json);

				if (*json == '-' || *json == '+')
				{
					ArrayFn::pushBack(number, *json);
					json = getNext(json);
				}
				while (isdigit(*json))
				{
					ArrayFn::pushBack(number, *json);
					json = getNext(json);
				}
			}

			// Ensure null terminated
			ArrayFn::pushBack(number, '\0');
			return parseDouble(ArrayFn::begin(number));
		}

		bool parseBool(const char* json)
		{
			RIO_ASSERT_NOT_NULL(json);

			switch (*json)
			{
				case 't':
				{
					json = getNext(json, 't');
					json = getNext(json, 'r');
					json = getNext(json, 'u');
					json = getNext(json, 'e');
					return true;
				}
				case 'f':
				{
					json = getNext(json, 'f');
					json = getNext(json, 'a');
					json = getNext(json, 'l');
					json = getNext(json, 's');
					json = getNext(json, 'e');
					return false;
				}
				default:
				{
					RIO_FATAL("Bad boolean");
					return false;
				}
			}
		}

		int32_t parseInt(const char* json)
		{
			return (int32_t)parseDouble(json);
		}

		float parseFloat(const char* json)
		{
			return (float)parseDouble(json);
		}

		void parseArray(const char* json, Array<const char*>& array)
		{
			RIO_ASSERT_NOT_NULL(json);

			if (*json == '[')
			{
				json = getNext(json, '[');
				json = skipSpaces(json);

				if (*json == ']')
				{
					json = getNext(json, ']');
					return;
				}

				while (*json)
				{
					ArrayFn::pushBack(array, json);

					json = skipValue(json);
					json = skipSpaces(json);

					if (*json == ']')
					{
						json = getNext(json, ']');
						return;
					}

					json = getNext(json, ',');
					json = skipSpaces(json);
				}
			}

			RIO_FATAL("Bad array");
		}

		void parseObject(const char* json, Map<DynamicString, const char*>& object)
		{
			RIO_ASSERT_NOT_NULL(json);

			if (*json == '{')
			{
				json = getNext(json, '{');

				json = skipSpaces(json);

				if (*json == '}')
				{
					getNext(json, '}');
					return;
				}

				while (*json)
				{
					DynamicString key;
					parseString(json, key);

					json = skipString(json);
					json = skipSpaces(json);
					json = getNext(json, ':');
					json = skipSpaces(json);

					MapFn::set(object, key, json);

					json = skipValue(json);
					json = skipSpaces(json);

					if (*json == '}')
					{
						getNext(json, '}');
						return;
					}

					json = getNext(json, ',');
					json = skipSpaces(json);
				}
			}

			RIO_FATAL("Bad object");
		}
	} // namespace Json

} // namespace Rio
