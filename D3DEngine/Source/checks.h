#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "logger.h"

#define ENGINE_DBG 1

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define ENGINE_ERROR(x) logger::customLog(x, 0, __FILENAME__, __LINE__);\
std::terminate();

#define ENGINE_ERROR_C(x, c) logger::customLog(x, c, __FILENAME__, __LINE__);\
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