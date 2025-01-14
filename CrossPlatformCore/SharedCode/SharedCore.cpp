#include "SharedCore.h"

#define PLATFORM_ANDROID 0
#define PLATFORM_IOS 1
#define PLATFORM_WINDOWS 2

char * SharedCore::getTemplateInfo()
{
#if PLATFORM == PLATFORM_IOS
	static char info[] = "Platform for iOS";
#elif PLATFORM == PLATFORM_ANDROID
	static char info[] = "Platform for Android";
#elif PLATFORM == PLATFORM_WINDOWS
	static char info[] = "Platform for Windows";
#else
	static char info[] = "Undefined platform";
#endif

	return info;
}

SharedCore::SharedCore()
{
}

SharedCore::~SharedCore()
{
}
