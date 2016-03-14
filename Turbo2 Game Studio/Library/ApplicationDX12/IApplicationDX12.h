#pragma once

#include "pch.h"

#include "IProgram.h"

namespace Application_DX12
{
	class IApplicationDX12
	{
	public:
		virtual IProgram *GetProgram() = 0;

	};
}