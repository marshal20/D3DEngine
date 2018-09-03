#pragma once

#define SAFE_RELEASE(x) if(x) { x->Release(); x = nullptr; }
#define SAFE_FREE(x) if(x) { delete x; x = nullptr; }
