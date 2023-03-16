#pragma once

//#include <pch.h>
//#include <locale>
#include <codecvt>
#include <cstring>
#include <memory>
#include <string>
#include <stdexcept>

namespace Turbo
{
    namespace Core
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

        template<typename ... Args>
        std::string string_format( const std::string& format, Args ... args )
        {
            int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
            //if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
            auto size = static_cast<size_t>( size_s );
            std::unique_ptr<char[]> buf( new char[ size ] );
            std::snprintf( buf.get(), size, format.c_str(), args ... );
            return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
        }
    }	//	namespace Turbo::Core
}	//	namespace Turbo
