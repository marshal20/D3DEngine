#include <CoreEngine/filesystem/image.h>

#include "../3rd_party/stb/stb_image.h"

namespace ce
{
	Image::Image()
	{
	}

	Image::~Image()
	{
	}

	void Image::load_from_memory(const Raw& raw)
	{
		/*m_image_data.data = (char*)stbi_load_from_memory((stbi_uc*)raw.data, raw.size,
			&m_size.x, &m_size.y, nullptr, 4);
		if (!m_image_data.data)
		{
			// TODO: Handle error.
		}

		m_image_data.size = m_size.x * m_size.y * 4 * sizeof(char);*/

		char* dataPtr;
		size_t dataSize;

		dataPtr = (char*)stbi_load_from_memory((stbi_uc*)raw.data, raw.size,
			&m_size.x, &m_size.y, nullptr, 4);
		if (dataPtr == nullptr)
		{
			// TODO: Handle error.
		}

		dataSize = sizeof(char) * 4 * m_size.x * m_size.y;
		m_data.resize(dataSize, 0);
		memcpy(&m_data[0], dataPtr, dataSize);

		stbi_image_free(dataPtr);
	}

	void Image::load_from_file(const char* file_name)
	{
		Raw raw;

		raw = file::load_file(file_name);
		load_from_memory(raw);
		file::free_raw(raw);
	}

	void Image::cleanup()
	{
		m_data.clear();
	}

	const std::vector<char>& Image::get_data() const
	{
		return m_data;
	}

	math::Vec2<int> Image::get_size() const
	{
		return m_size;
	}
}