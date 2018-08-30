#pragma once

#define CHECK_D3D_CALLS 1

#if CHECK_D3D_CALLS
#include <stdio.h>
#define CHECK_HR(x) if(FAILED(x)) { printf("Call failed, HR: 0x%X \nFile: %s \nLine: %d\n", x, __FILE__, __LINE__); }
#else
#define CHECK_HR(x) 
#endif