#pragma once
#if (defined _WIN32) || (defined WIN32)
#	define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
//#include "IniFile.h"
#if (defined _WIN32) || (defined WIN32)
#	pragma comment(lib,"iphlpapi.lib")
#	pragma comment(lib,"ws2_32")
#endif
