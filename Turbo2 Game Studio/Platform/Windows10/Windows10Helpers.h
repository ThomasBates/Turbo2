#pragma once

#include <pch.h>
#include <locale>
#include <codecvt>
#include <string>

using namespace Platform;

namespace Turbo 
{
	namespace Platform
	{
		namespace Windows10
		{
			inline void ThrowIfFailed(HRESULT hr)
			{
				if (FAILED(hr))
				{
					// Set a breakpoint on this line to catch Win32 API errors.
					throw Exception::CreateException(hr);
				}
			}

			inline std::wstring ToWString(std::string value)
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				return converter.from_bytes(value);
			}

			inline std::string ToString(std::wstring value)
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				return converter.to_bytes(value);
			}


			// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
			inline float ConvertDipsToPixels(float dips, float dpi)
			{
				static const float dipsPerInch = 96.0f;
				return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
			}

		}	//	namespace Turbo::Platform::Windows10
	}	//	namespace Turbo::Platform
}	//	namespace Turbo
