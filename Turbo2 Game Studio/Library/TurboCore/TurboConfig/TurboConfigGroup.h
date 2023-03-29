
#pragma once

#include <pch.h>

#include <ITurboConfigGroup.h>

namespace Turbo
{
	namespace Core
	{
		namespace Config
		{
			class TurboConfigGroup : public ITurboConfigGroup
			{
			public:
				TurboConfigGroup(std::shared_ptr<ITurboConfigProvider> provider, std::string name);
				virtual ~TurboConfigGroup() {}

				//  ITurboConfigGroup Events ---------------------------------------------------------------------------
				virtual std::shared_ptr<ITurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>> OnValueChanged() { return _onValueChanged; }
				
				//  ITurboConfigGroup Properties -----------------------------------------------------------------------
				virtual std::vector<std::shared_ptr<ITurboConfigGroup>> GetGroups() { return _groups; }
				virtual std::vector<std::shared_ptr<ITurboConfigItem>> GetItems() { return _items; }

				//  ITurboConfigProvider Properties --------------------------------------------------------------------
				virtual std::string GetName() { return _name; }
				virtual std::string GetFullName();
				virtual bool IsInBatch() { return _provider->IsInBatch(); }
				virtual bool IsInitialized() { return _provider->IsInitialized(); }
				
				//  ITurboConfigGroup Methods --------------------------------------------------------------------------
				virtual void AddGroup(std::shared_ptr<ITurboConfigGroup> group);
				virtual void AddItem(std::shared_ptr<ITurboConfigItem> item);
				
				virtual void RemoveGroup(std::shared_ptr<ITurboConfigGroup> group);
				virtual void RemoveItem(std::shared_ptr<ITurboConfigItem> item);
				
				virtual std::shared_ptr<ITurboConfigGroup> FindGroup(std::string groupName);
				virtual std::shared_ptr<ITurboConfigItem> FindItem(std::string itemName);
				
				virtual void CleanUpGroups();
				virtual void CleanUpItems();
				
				//  ITurboConfigProvider Methods -----------------------------------------------------------------------
				virtual void WriteString(std::string path, std::string value, bool isDefaultValue);
				virtual std::string ReadString(std::string path);
				
				virtual std::vector<std::string> GetChildGroupNames(std::string path);
				virtual std::vector<std::string> GetChildItemNames(std::string path);
				
				virtual void CleanUpChildGroups();
				virtual void CleanUpChildItems();
				
				virtual void BeginBatch() { _provider->BeginBatch(); }
				virtual void EndBatch() { _provider->EndBatch(); }

				//  Event Handlers -------------------------------------------------------------------------------------
				void HandleOnValueChanged(void *sender, std::shared_ptr<TurboConfigValueChangedEventArgs> args);

			private:
				std::shared_ptr<ITurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>> _onValueChanged;
				
				std::shared_ptr<ITurboConfigProvider> _provider;
				
				std::string _name;
				std::string _fullName;

				std::vector<std::shared_ptr<ITurboConfigGroup>> _groups;
				std::vector<std::shared_ptr<ITurboConfigItem>> _items;
            };
		}
	}
}