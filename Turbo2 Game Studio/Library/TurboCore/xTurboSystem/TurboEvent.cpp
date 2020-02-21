#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Core
	{
		namespace System
		{
			template <class T>
			class ITurboEvent
			{
			public:
				virtual void Subscribe(void *receiver, void (*eventHandler)(void*, T)) = 0;
				virtual void Unsubscribe(void *receiver, void (*eventHandler)(void*, T)) = 0;
				virtual void Publish(void *sender, T args) = 0;
				virtual int Count() = 0;
			};
		}
	}
}