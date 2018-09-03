#pragma once

#include <vector>

#include "file.h"
#include "../math/vec2.h"

namespace ce
{
	class Image
	{
	private:
		std::vector<char> m_data;
		math::Vec2<int> m_size = { 0, 0 };

	public:
		Image();
		~Image();

		void load_from_memory(const Raw& raw);
		void load_from_file(const char* file_name);
		void cleanup();

		const std::vector<char>& get_data() const;
		math::Vec2<int> get_size() const;
	};
}