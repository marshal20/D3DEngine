#include <CoreEngine/filesystem/file.h>

#include <fstream>

namespace ce
{
	namespace file 
	{
		Raw load_file(const char* file_name)
		{
			Raw raw;
			std::ifstream in_file;

			in_file.open(file_name, std::ios_base::binary);
			if (!in_file.is_open())
			{
				// TODO: Handle error.
			}

			in_file.seekg(0, std::ios::end);
			raw.size = (size_t)in_file.tellg();
			in_file.seekg(0, std::ios::beg);
			raw.size -= (size_t)in_file.tellg();

			raw.data = new char[raw.size];
			in_file.read(raw.data, raw.size);

			in_file.close();

			return raw;
		}

		void free_raw(Raw& raw)
		{
			if (raw.data)
			{
				delete[] raw.data;
			}

			raw.data = nullptr;
			raw.size = 0;
		}
	}
}