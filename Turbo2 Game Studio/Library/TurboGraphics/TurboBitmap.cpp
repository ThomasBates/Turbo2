
#include <pch.h>

#include <TurboBitmap.h>

//  Constructors & Destructors -----------------------------------------------------------------------------------------

Turbo::Graphics::TurboBitmap::TurboBitmap(ITurboCanvas *canvas, void *data)
{
    Canvas(canvas);
    Data(data);
}

//  ITurboImage Properties ---------------------------------------------------------------------------------------------

void Turbo::Graphics::TurboBitmap::Data(void* data)
{
    _bmpFile = nullptr;
    _data = (unsigned char*)data;
    if (_data)
        _bmpFile = (BmpFile*)(_data + 2);
}

//  ITurboImage Methods ------------------------------------------------------------------------------------------------

void Turbo::Graphics::TurboBitmap::RenderToCanvas(int width, int height, IMG_STYLE style)
{
    if (_canvas && _bmpFile)
    {
        int actualWidth;
        if (width > 0)
            actualWidth = width;
        else if (_width > 0)
            actualWidth = _width;
        else
            actualWidth = _bmpFile->info.width;

        int actualHeight;
        if (height > 0)
            actualHeight = height;
        else if (_height > 0)
            actualHeight = _height;
        else
            actualHeight = _bmpFile->info.height;

        if (actualWidth != _canvas->Width() ||
            actualHeight!= _canvas->Height() ||
            style       != _style)
        {
            _canvas->Width(actualWidth);
            _canvas->Height(actualHeight);
            _style = style;

            if (actualWidth  == (int)(_bmpFile->info.width) &&
                actualHeight == (int)(_bmpFile->info.height))
                RenderSimple();
            else
                RenderScale();
        }
    }
}

//  Private Methods ----------------------------------------------------------------------------------------------------

void Turbo::Graphics::TurboBitmap::RenderSimple()
{
    _canvas->Clear();

    unsigned char *pixelData = ((unsigned char*)_bmpFile) + _bmpFile->header.offset - 2;
    unsigned char *firstPixel = pixelData;

    int pixelSize = _bmpFile->info.bitDepth / 8;
    int realWidth = (_bmpFile->info.width * pixelSize);
    if (realWidth % 4)
        realWidth += 4 - realWidth % 4;

    ITurboPen *pen = _canvas->Pen();

    for (unsigned int x = 0; x < _bmpFile->info.width;  x ++)
    for (unsigned int y = 0; y < _bmpFile->info.height; y++)
    {
        int yy = _bmpFile->info.height - 1 - y;

        unsigned char *rgb = pixelData + yy * realWidth + x * pixelSize;

        if (pixelSize == 3)
        {
            int drawIt = 1;
            if ((_style & IMG_TRANSPARENT) &&
                (rgb[0] == firstPixel[0]) &&
                (rgb[1] == firstPixel[1]) &&
                (rgb[2] == firstPixel[2]))
                drawIt = 0;

            if (drawIt)
            {
                pen->SetRGB(rgb[2], rgb[1], rgb[0]);
                _canvas->SetPixel(x, yy, pen->GetColor());
            }
        }
        else if (pixelSize == 4)
        {
            pen->SetRGBA(rgb[2], rgb[1], rgb[0], rgb[3]);
            _canvas->SetPixel(x, yy, pen->GetColor());
        }
    }
}

void Turbo::Graphics::TurboBitmap::RenderScale()
{
    int px0 = 0;
    int py0 = 0;
    int px1 = 0;
    int py1 = 0;
    double xScale;
    double yScale;
    int canvasWidth = _canvas->Width();
    int canvasHeight = _canvas->Height();

    _canvas->Clear();

    unsigned char *pixelData = ((unsigned char*)_bmpFile) + _bmpFile->header.offset - 2;
    int realWidth = (_bmpFile->info.width * 3);
    if (realWidth % 4)
        realWidth += 4 - realWidth % 4;

    ITurboPen *pen = _canvas->Pen();

    switch (_style & IMG_DRAWSTYLE)
    {
        case IMG_NORMAL:
            xScale = 1.0;
            yScale = 1.0;
            if (_style & IMG_LEFT)
            {
                px0 = 0;
                px1 = _bmpFile->info.width - 1;
            }
            else if (_style & IMG_RIGHT)
            {
                px1 = (canvasWidth - 1);
                px0 = (canvasWidth - 1) - _bmpFile->info.width + 1;
            }
            else
            {
                px0 = (canvasWidth - _bmpFile->info.width) / 2;
                px1 = px0 + _bmpFile->info.width - 1;
            }
            if (_style & IMG_TOP)
            {
                py0 = 0;
                py1 = _bmpFile->info.height - 1;
            }
            else if (_style & IMG_BOTTOM)
            {
                py1 = (canvasHeight - 1);
                py0 = (canvasHeight - 1) - _bmpFile->info.height + 1;
            }
            else
            {
                py0 = (canvasHeight - _bmpFile->info.height) / 2;
                py1 = py0 + _bmpFile->info.height - 1;
            }
            break;

        case IMG_ZOOM:
            xScale = _bmpFile->info.width  * 1.0 / canvasWidth;
            yScale = _bmpFile->info.height * 1.0 / canvasHeight;

            px0 = 0;
            px1 = (canvasWidth - 1);
            py0 = 0;
            py1 = (canvasHeight - 1);

            if (xScale < yScale)
            {
                xScale = yScale;
                px0 = (int)((canvasWidth - _bmpFile->info.width / xScale) / 2);
                px1 = (int)((canvasWidth + _bmpFile->info.width / xScale) / 2);
            }
            else if (yScale < xScale)
            {
                yScale = xScale;
                py0 = (int)((canvasHeight - _bmpFile->info.height / yScale) / 2);
                py1 = (int)((canvasHeight + _bmpFile->info.height / yScale) / 2);
            }
            break;

        case IMG_TILE:
            xScale = 1.0;
            yScale = 1.0;
            px0 = 0;
            px1 = (canvasWidth - 1);
            py0 = 0;
            py1 = (canvasHeight - 1);
            break;

        case IMG_STRETCH:
            xScale = _bmpFile->info.width  * 1.0 / canvasWidth;
            yScale = _bmpFile->info.height * 1.0 / canvasHeight;
            px0 = 0;
            px1 = (canvasWidth - 1);
            py0 = 0;
            py1 = (canvasHeight - 1);
            break;

        default:
            //  To please the lint overlords.
            xScale = 0;
            yScale = 0;
            break;
    }


    for (int sx = px0; sx <= px1; sx++)
    for (int sy = py0; sy <= py1; sy++)
    {
        int ix = (int)((sx - px0) * xScale);
        int iy = (int)((sy - py0) * yScale);

        ix = ix % _bmpFile->info.width;
        iy = iy % _bmpFile->info.height;

        iy = _bmpFile->info.height - 1 - iy;

        unsigned char *rgb = pixelData + ix * 3 + iy * realWidth;
        int drawIt = 1;
        if ((_style & IMG_TRANSPARENT) &&
            (rgb[0] == pixelData[0]) &&
            (rgb[1] == pixelData[1]) &&
            (rgb[2] == pixelData[2]))
            drawIt = 0;

        if (drawIt)
        {
            pen->SetRGB(rgb[2], rgb[1], rgb[0]);
            _canvas->SetPixel(sx, sy, pen->GetColor());
        }
    }
}

