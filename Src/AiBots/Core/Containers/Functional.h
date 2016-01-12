// Copyright (c) 2015 Volodymyr Syvochka

#pragma once

namespace Rio
{

	template<typename T>
	struct equalTo
	{
		bool operator()(const T& a, const T& b) const
		{
			return a == b;
		};
	};

	template<typename T>
	struct notEqualTo
	{
		bool operator()(const T& a, const T& b) const
		{
			return a != b;
		};
	};

	template <typename T>
	struct greater
	{
		bool operator()(const T& a, const T& b) const
		{
			return a > b;
		};
	};

	template<typename T>
	struct less
	{
		bool operator()(const T& a, const T& b) const
		{
			return a < b;
		};
	};

	template<typename T>
	struct greaterEqual
	{
		bool operator()(const T& a, const T& b) const
		{
			return a >= b;
		};
	};

	template<typename T>
	struct lessEqual
	{
		bool operator()(const T& a, const T& b) const
		{
			return a <= b;
		};
	};

} // namespace Rio
