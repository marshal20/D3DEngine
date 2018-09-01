#pragma once

#include <vector>
#include <string>

struct ID3D11InputLayout;

namespace ce
{
	class Layout
	{
	public:
		struct Element
		{
			enum class Type { FLOAT = 1, INT };
			std::string name;
			Type type;
			int count;

			Element(const std::string& name_, Type type_, int count_)
				: name(name_), type(type_), count(count_)
			{}
		};

	private:
		std::vector<Element> m_list;
		ID3D11InputLayout* m_layout = nullptr;

	public:
		Layout();
		~Layout();

		void create();
		void cleanup();

		void push(const Element& element);

		void use();
	};

}