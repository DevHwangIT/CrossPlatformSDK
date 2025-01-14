#pragma once

#if defined(__WIN32__) || defined(__WIN64__)
	#define EXPORT extern "C" __declspec(dllexport)
#elif defined(__ANDROID__)
	#define EXPORT extern "C" JNIEXPORT
#elif defined(__IOS__)
	#define EXPORT extern "C"
#else
	#define EXPORT extern "C"
#endif