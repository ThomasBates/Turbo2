
#include <pch.h>

#include <TurboConfigGroup.h>
#include <TurboEventHandler.h>

using namespace Turbo::Core::Config;

TurboConfigGroup::TurboConfigGroup(std::shared_ptr<ITurboConfigProvider> provider, std::string name) :
	_provider(std::move(provider)),
	_name(std::move(name))
{
	_onValueChanged = std::shared_ptr<ITurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>>(new TurboEvent<std::shared_ptr<TurboConfigValueChangedEventArgs>>());
}

std::string TurboConfigGroup::GetFullName()
{
	if (_fullName.empty())
	{
		auto providerFullName = _provider->GetFullName();
		_fullName = providerFullName.empty()
				? _name
				: providerFullName + "." + _name;
	}
	return _fullName;
}

void TurboConfigGroup::AddGroup(std::shared_ptr<ITurboConfigGroup> group)
{
	_groups.push_back(group);

	group->OnValueChanged()->Subscribe(std::shared_ptr<ITurboEventHandler<std::shared_ptr<TurboConfigValueChangedEventArgs>>>(
			new TurboEventHandler<TurboConfigGroup, std::shared_ptr<TurboConfigValueChangedEventArgs>>(this, &TurboConfigGroup::HandleOnValueChanged)));
}

void TurboConfigGroup::AddItem(std::shared_ptr<ITurboConfigItem> item)
{
	_items.push_back(item);

	item->OnValueChanged()->Subscribe(std::shared_ptr<ITurboEventHandler<std::shared_ptr<TurboConfigValueChangedEventArgs>>>(
			new TurboEventHandler<TurboConfigGroup, std::shared_ptr<TurboConfigValueChangedEventArgs>>(this, &TurboConfigGroup::HandleOnValueChanged)));
}

void TurboConfigGroup::RemoveGroup(std::shared_ptr<ITurboConfigGroup> group)
{
	_groups.erase(std::remove(_groups.begin(), _groups.end(), group), _groups.end());
}

void TurboConfigGroup::RemoveItem(std::shared_ptr<ITurboConfigItem> item)
{
	_items.erase(std::remove(_items.begin(), _items.end(), item), _items.end());
}

std::shared_ptr<ITurboConfigGroup> TurboConfigGroup::FindGroup(std::string groupName)
{
	for (auto &group : _groups)
	{
		if (group->GetName() == groupName)
		{
			return group;
		}
	}
	return nullptr;
}

std::shared_ptr<ITurboConfigItem> TurboConfigGroup::FindItem(std::string itemName)
{
	for (auto &item : _items)
	{
		if (item->GetName() == itemName)
		{
			return item;
		}
	}
	return nullptr;
}

void TurboConfigGroup::CleanUpGroups()
{
}

void TurboConfigGroup::CleanUpItems()
{
}

void TurboConfigGroup::WriteString(std::string path, std::string value, bool isDefaultValue)
{
	_provider->WriteString(GetName() + "." + path, value, isDefaultValue);
}

std::string TurboConfigGroup::ReadString(std::string path)
{
	return _provider->ReadString(GetName() + "." + path);
}

std::vector<std::string> TurboConfigGroup::GetChildGroupNames(std::string path)
{
	return _provider->GetChildGroupNames(GetName() + "." + path);
}

std::vector<std::string> TurboConfigGroup::GetChildItemNames(std::string path)
{
	return _provider->GetChildItemNames(GetName() + "." + path);
}

void TurboConfigGroup::CleanUpChildGroups()
{
}

void TurboConfigGroup::CleanUpChildItems()
{
}

void TurboConfigGroup::HandleOnValueChanged(void *sender, std::shared_ptr<TurboConfigValueChangedEventArgs> args)
{
	_onValueChanged->Publish(sender, args);
}
