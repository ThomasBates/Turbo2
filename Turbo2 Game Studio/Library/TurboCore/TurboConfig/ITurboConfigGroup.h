
#pragma once

#include <pch.h>

#include <ITurboConfigItem.h>
#include <ITurboConfigProvider.h>
#include <ITurboEvent.h>

#include <TurboConfigValueChangedEventArgs.h>

namespace Turbo
{
	namespace Core
	{
		namespace Config
		{
			class ITurboConfigGroup : public ITurboConfigProvider
			{
			public:
				//  ITurboConfigGroup Events ---------------------------------------------------------------------------
				virtual std::shared_ptr<ITurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>> OnValueChanged() = 0;
				
				//  ITurboConfigGroup Properties -----------------------------------------------------------------------
				virtual std::vector<std::shared_ptr<ITurboConfigGroup>> GetGroups() = 0;
				virtual std::vector<std::shared_ptr<ITurboConfigItem>> GetItems() = 0;

				//  ITurboConfigGroup Methods --------------------------------------------------------------------------
				virtual void AddGroup(std::shared_ptr<ITurboConfigGroup> group) = 0;
				virtual void AddItem(std::shared_ptr<ITurboConfigItem> item) = 0;
				
				virtual void RemoveGroup(std::shared_ptr<ITurboConfigGroup> group) = 0;
				virtual void RemoveItem(std::shared_ptr<ITurboConfigItem> item) = 0;
				
				virtual std::shared_ptr<ITurboConfigGroup> FindGroup(std::string groupName) = 0;
				virtual std::shared_ptr<ITurboConfigItem> FindItem(std::string itemName) = 0;
				
				virtual void CleanUpGroups() = 0;
				virtual void CleanUpItems() = 0;
			};
		}
	}
}