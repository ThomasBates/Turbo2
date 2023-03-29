
#pragma once

#include <pch.h>

#include <ITurboConfigTypedItem.h>
#include <TurboConfigGroup.h>
#include <ITurboDebug.h>

using namespace Turbo::Core::Config;
using namespace Turbo::Core::Debug;

class UserOptions : public TurboConfigGroup
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	UserOptions(std::shared_ptr<ITurboDebug> debug, std::shared_ptr<ITurboConfigProvider> provider, std::string name);
	virtual ~UserOptions() {}

    virtual std::shared_ptr<ITurboConfigTypedItem<bool>> InvertedMouse() { return _invertedMouse; }
    virtual std::shared_ptr<ITurboConfigTypedItem<bool>> SoundEffectsOn() { return _soundEffectsOn; }

private:
    std::shared_ptr<ITurboConfigTypedItem<bool>> _invertedMouse;
    std::shared_ptr<ITurboConfigTypedItem<bool>> _soundEffectsOn;
};
