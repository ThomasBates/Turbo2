
#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Core
	{
		namespace Config
		{
			class ITurboConfigProvider
			{
			public:
				//  ITurboConfigProvider Properties --------------------------------------------------------------------
				virtual std::string GetName() = 0;
				virtual std::string GetFullName() = 0;
				virtual bool IsInBatch() = 0;
				virtual bool IsInitialized() = 0;

				//  ITurboConfigProvider Methods -----------------------------------------------------------------------
				virtual void WriteString(std::string path, std::string value, bool isDefaultValue) = 0;
				virtual std::string ReadString(std::string path) = 0;
				
				virtual std::vector<std::string> GetChildGroupNames(std::string path) = 0;
				virtual std::vector<std::string> GetChildItemNames(std::string path) = 0;
				
				virtual void CleanUpChildGroups() = 0;
				virtual void CleanUpChildItems() = 0;
				
				virtual void BeginBatch() = 0;
				virtual void EndBatch() = 0;
			};
		}
	}
}