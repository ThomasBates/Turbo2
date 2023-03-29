#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboConfigProvider.h>

using namespace Turbo::Core::Config;
using namespace Turbo::Core::Debug;

namespace Turbo
{
	namespace Platform
	{
		namespace AndroidNDK
		{
			class AndroidNDKConfigProvider : public ITurboConfigProvider
			{
			public:
				//  Constructors and Destructors -----------------------------------------------------------------------
				AndroidNDKConfigProvider(std::shared_ptr<ITurboDebug> debug, std::string name);
				virtual ~AndroidNDKConfigProvider() {};

				//  ITurboConfigProvider Properties --------------------------------------------------------------------
				virtual std::string GetName() { return _name; }
				virtual std::string GetFullName() { return ""; }
				virtual bool IsInBatch() { return _batchCount > 0; }
				virtual bool IsInitialized();

				//  ITurboConfigProvider Methods -----------------------------------------------------------------------
				virtual void WriteString(std::string path, std::string value, bool isDefaultValue);
				virtual std::string ReadString(std::string path);

				virtual std::vector<std::string> GetChildGroupNames(std::string path) { return std::vector<std::string>(); }
				virtual std::vector<std::string> GetChildItemNames(std::string path) { return std::vector<std::string>(); }

				virtual void CleanUpChildGroups() {}
				virtual void CleanUpChildItems() {}

				virtual void BeginBatch();
				virtual void EndBatch();

			private:
				std::shared_ptr<ITurboDebug> _debug;
				std::string _name;

				bool _isDocumentLoaded = false;
				std::string _location;
				std::map<std::string, std::string> _dictionary;

				bool _isDirty = false;
				int _batchCount = 0;

                void SaveDocument();
                void LoadDocument();

                static std::string SanitizeStringValue(std::string value);
                static std::string RestoreStringValue(std::string value);
                static void ReplaceAll(std::string &str, const std::string &from, const std::string &to);
            };
		}
	}
}
