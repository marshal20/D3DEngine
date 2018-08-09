#pragma once
#include "checks.h"
#include "logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace checks
{
	void D3D11CALL_WRN(long hr, const char* message)
	{
		if (FAILED(hr)) {
			ENGINE_WRN(message, hr);
		}
	}

	void D3D11CALL_ERR(long hr, const char* message)
	{
		if (FAILED(hr)){
			ENGINE_ERR(message, hr);
		}
	}

	void ENGINE_WRN(const char* message, int error_code)
	{
		Logger::CUSTOMLOG(2, message, error_code);
	}

	void ENGINE_ERR(const char* message, int error_code)
	{
		Logger::CUSTOMLOG(1, message, error_code);
		std::terminate();
	}
} // checks