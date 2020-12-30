
#pragma once

#include <pch.h>

#include <ITurboCanvas.h>

namespace Turbo
{
    namespace Graphics
    {
        enum IMG_STYLE: unsigned short
        {
            IMG_NORMAL = 0x0000,
            IMG_ZOOM = 0x0001,
            IMG_TILE = 0x0002,
            IMG_STRETCH = 0x0004,
            IMG_DRAWSTYLE = 0x000F,

            IMG_LEFT = 0x0010,
            IMG_RIGHT = 0x0020,
            IMG_TOP = 0x0040,
            IMG_BOTTOM = 0x0080,
            IMG_ALIGNMENT = 0x00F0,

            IMG_TRANSPARENT = 0x0100,
        };

        class ITurboImage
        {
        public:
            //  ITurboImage Properties ---------------------------------------------------------------------------------
            virtual ITurboCanvas *Canvas() = 0;
            virtual void Canvas(ITurboCanvas *canvas) = 0;

            virtual void* Data() = 0;
            virtual void Data(void* data) = 0;

            virtual int Width() = 0;
            virtual void Width(int width) = 0;

            virtual int Height() = 0;
            virtual void Height(int height) = 0;

            virtual IMG_STYLE Style() = 0;
            virtual void Style(IMG_STYLE style) = 0;

            //  ITurboImage Methods ------------------------------------------------------------------------------------
            virtual void RenderToCanvas(int width = 0, int height = 0, IMG_STYLE style = IMG_NORMAL) = 0;
        };
    }
}
