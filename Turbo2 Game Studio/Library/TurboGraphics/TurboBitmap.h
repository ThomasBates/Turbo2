
#pragma once

#include <ITurboImage.h>

namespace Turbo
{
    namespace Graphics
    {
        typedef struct
        {
            unsigned int fileSize;
            unsigned int reserved;
            unsigned int offset;
        } BmpHeader;

        typedef struct
        {
            unsigned int headerSize;
            unsigned int width;
            unsigned int height;
            unsigned short planeCount;
            unsigned short bitDepth;
            unsigned int compression;
            unsigned int compressedImageSize;
            unsigned int horizontalResolution;
            unsigned int verticalResolution;
            unsigned int numColors;
            unsigned int importantColors;
        } BmpImageInfo;

        typedef struct
        {
            BmpHeader header;
            BmpImageInfo info;
        } BmpFile;

        class TurboBitmap : public ITurboImage
        {
        public:

            //  Constructors & Destructors -----------------------------------------------------------------------------
            TurboBitmap(ITurboCanvas *canvas = 0, void *data = 0);
            virtual ~TurboBitmap() {}

            //  ITurboImage Properties ---------------------------------------------------------------------------------
            virtual ITurboCanvas* Canvas() { return _canvas; }
            virtual void Canvas(ITurboCanvas *canvas) { _canvas = canvas; }

            virtual void* Data() { return (void*)_data; }
            virtual void Data(void* data);

            virtual int Width() { return _width; }
            virtual void Width(int width) { _width = width; }

            virtual int Height() { return _height; }
            virtual void Height(int height) { _height = height; }

            virtual IMG_STYLE Style() { return _style; }
            virtual void Style(IMG_STYLE style) { _style = style; }

            //  ITurboImage Methods ------------------------------------------------------------------------------------
            virtual void RenderToCanvas(int width, int height, IMG_STYLE style);

        private:
            unsigned char* _data;
            ITurboCanvas* _canvas;

            int _width {0};
            int _height {0};
            IMG_STYLE _style {IMG_NORMAL};

            BmpFile* _bmpFile;

            void RenderSimple();
            void RenderScale();
        };
    }
}
