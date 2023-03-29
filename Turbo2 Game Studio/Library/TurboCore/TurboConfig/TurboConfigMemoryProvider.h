
#pragma once

#include <pch.h>

#include <ITurboConfigProvider.h>

namespace Turbo
{
	namespace Core
	{
		namespace Config
		{
			class TurboConfigMemoryProvider : public ITurboConfigProvider
			{
			public:
				TurboConfigMemoryProvider(std::string name);
				virtual ~TurboConfigMemoryProvider() {}
			
				//  ITurboConfigProvider Properties --------------------------------------------------------------------
				virtual std::string GetName() { return _name; }
				virtual std::string GetFullName() { return ""; }
				virtual bool IsInBatch() { return false; }
				virtual bool IsInitialized() { return true; }
				
				//  ITurboConfigProvider Methods -----------------------------------------------------------------------
				virtual void WriteString(std::string path, std::string value, bool isDefaultValue);
				virtual std::string ReadString(std::string path);
				
				virtual std::vector<std::string> GetChildGroupNames(std::string path) { return std::vector<std::string>(); }
				virtual std::vector<std::string> GetChildItemNames(std::string path) { return std::vector<std::string>(); }
				
				virtual void CleanUpChildGroups() {}
				virtual void CleanUpChildItems() {}
				
				virtual void BeginBatch() {}
				virtual void EndBatch() {}
				
			private:
				std::map<std::string, std::string> _dictionary;

				std::string _name;
			};
		}
	}
}