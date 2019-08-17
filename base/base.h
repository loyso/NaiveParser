#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define NOMINMAX

#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#	ifndef DBG_NEW
#		define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#		define SS_NEW DBG_NEW
#	else
#		define SS_NEW new
#	endif
#else  // NDEBUG
#	define SS_NEW new
#endif

#include <assert.h>
#define SS_ASSERT assert

namespace base
{
}
