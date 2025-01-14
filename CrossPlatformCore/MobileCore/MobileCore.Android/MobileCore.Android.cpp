#include "pch.h"
#include "SharedCore.h"

char * AndroidInfo()
{
	return SharedCore::getTemplateInfo();
}