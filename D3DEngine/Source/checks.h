#pragma once

namespace checks
{
	extern void D3D11CALL_WRN(long hr, const char* message = "");
	extern void D3D11CALL_ERR(long hr, const char* message = "");

	extern void ENGINE_WRN(const char* message, int error_code = 0);
	extern void ENGINE_ERR(const char* message, int error_code = 0);
} // checks

/*
#define ENGINE_DBG 1

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define ENGINE_ERROR(x) Logger::CUSTOMLOG(0, x, 0, __FILENAME__, __LINE__);\
std::terminate();

#define ENGINE_ERROR_C(x, c) Logger::CUSTOMLOG(0, x, c, __FILENAME__, __LINE__);\
std::terminate();

#define D3D11CALL(x) {\
HRESULT hr = x;\
if(FAILED(hr)) {\
ENGINE_ERROR_C("Failed D3D11 call.", hr);\
}\
}

#if !ENGINE_DBG
#undef ENGINE_ERROR
#undef ENGINE_ERROR_C
#undef D3D11CALL
#define ENGINE_ERROR(x) 
#define ENGINE_ERROR_C(x, p) 
#define D3D11CALL(x) x
#endif
*/