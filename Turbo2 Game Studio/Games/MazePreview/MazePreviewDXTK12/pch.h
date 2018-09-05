//
// pch.h
// Header for standard system include files.
//

#pragma once

// Use the C++ standard templated min/max
#define NOMINMAX

#include <wrl.h>

#include <d3d12.h>

#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>

#include "d3dx12.h"

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <stdio.h>
#include <pix.h>

//#include <wrl.h>
#include <wrl/client.h>
//#include <d3d12.h>
//#include <d3dx12.h>
//#include <dxgi1_4.h>
#include <d3d11_3.h>
//#include <pix.h>
//#include <DirectXColors.h>
//#include <DirectXMath.h>
//#include <memory>
#include <vector>
#include <agile.h>
#include <concrt.h>
#include <map>

#include <ppltasks.h>
#include <synchapi.h>
#include <iostream>
#include <sstream>
#include <string>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <minwinbase.h>
#include <codecvt>


//	https://github.com/Microsoft/DirectXTK12/wiki/Adding-the-DirectX-Tool-Kit
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DescriptorHeap.h"
#include "DirectXHelpers.h"
#include "EffectPipelineStateDescription.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "RenderTargetState.h"
#include "ResourceUploadBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"


#ifdef _DEBUG
#include <dxgidebug.h>
#endif

/*
namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        virtual const char* what() const override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
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
}
*/