#pragma once

#include <pch.h>
#include <locale>
#include <codecvt>
#include <string>

//using namespace Platform;

namespace Turbo
{
    namespace Platform
    {
        namespace AndroidNDK
        {
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
        }	//	namespace Turbo::Platform::Windows10
    }	//	namespace Turbo::Platform
}	//	namespace Turbo
