#pragma once

#include <pch.h>

namespace Turbo
{
	namespace Core
	{
		namespace System
		{
			template <class T>
			class TurboEvent : public ITurboEvent<T>
			{
			public:
				virtual void Subscribe(void *receiver, void (*eventHandler)(void*, T));
				virtual void Unsubscribe(void *receiver, void (*eventHandler)(void*, T));
				virtual void Publish(void *sender, T args);
				virtual int Count() = 0;
			};
		}
	}
}