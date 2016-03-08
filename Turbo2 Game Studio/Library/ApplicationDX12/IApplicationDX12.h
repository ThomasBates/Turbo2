#pragma once

#include "pch.h"


#include "IProgram.h"

class IApplicationDX12
{
public:
	virtual IProgram *GetProgram() = 0;

};
