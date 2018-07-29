#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "logger.h"

#define ENGINE_DBG 1

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define ENGINE_ERROR(x) logger::customLog(x, __FILENAME__, __LINE__);\
std::terminate();

#define D3D11CALL(x) if(FAILED(x)) {\
ENGINE_ERROR("Failed D3D11 call.");\
}

#if !ENGINE_DBG
#undef ENGINE_ERROR
#undef D3D11CALL
#define ENGINE_ERROR(x) 
#define D3D11CALL(x) x
#endif