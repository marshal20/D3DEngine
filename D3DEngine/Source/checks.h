#pragma once

namespace checks
{
	extern void D3D11CALL_WRN(long hr, const char* message = "");
	extern void D3D11CALL_ERR(long hr, const char* message = "");

	extern void ENGINE_WRN(const char* message, int error_code = 0);
	extern void ENGINE_ERR(const char* message, int error_code = 0);
} // checks
