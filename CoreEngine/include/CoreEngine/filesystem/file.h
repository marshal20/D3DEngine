#pragma once

namespace ce
{
	struct Raw
	{
		char* data;
		size_t size;
		Raw() : data(nullptr), size(0) {}
		Raw(char* data_, size_t size_) : data(data_), size(size_) {}
	};

	namespace file 
	{
		extern Raw load_file(const char* file_name);
		extern void free_raw(Raw& raw);
	}
}