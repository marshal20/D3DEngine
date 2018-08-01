#pragma once
#include <vector>
#include <string>

struct VertexLayout
{
	enum class Type { FLOAT, INT};
	struct Element { Type type; size_t count; std::string name; };
	std::vector<Element> elements;

	template<typename T>
	void push(size_t count, const std::string& name)
	{
		static_assert(false, "Unsupported vertex layout type.");
	}

	template<>
	void push<float>(size_t count, const std::string& name)
	{
		elements.push_back({ Type::FLOAT, count, name });
	}

	template<>
	void push<int>(size_t count, const std::string& name)
	{
		elements.push_back({ Type::INT, count, name });
	}

};
