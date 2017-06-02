
//#define _CRT_SECURE_NO_WARNINGS 1
//#pragma warning(disable:4996)

#include <pch.h>

#include <TurboBitmap.h>

//  ============================================================================
//  DSBitmap
//  ============================================================================

//  ============================================================================
//  Constructors
//  ============================================================================

Turbo::Graphics::TurboBitmap::TurboBitmap(ITurboCanvas *canvas, void *data)
{
	SetCanvas(canvas);
	Data(data);

	_width	= 0;
	_height	= 0;
	_style	= 0;
}

Turbo::Graphics::TurboBitmap::~TurboBitmap()
{
}

void Turbo::Graphics::TurboBitmap::Data(void* data)
{
	_bmpFile = 0;
	_data = (byte*)data;
	if (_data)
		_bmpFile = (BmpFile*)(_data + 2);
}

//  ============================================================================
//  Public Access Methods
//  ============================================================================

/*
int	Turbo::Graphics::TurboBitmap::LoadFromFile(std::string fileName)
{
	FILE *file;
	char *buffer;
	unsigned long fileLen;

	delete [] _data;

	//Open file
	//file = fopen(fileName, "rb");
	fopen_s(&file, fileName.c_str(), "rb");
	if (!file)
	{
		//fprintf(stderr, "Unable to open file %s", fileName);
		return 0;
	}
	
	//Get file length
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate memory
	buffer = new char[fileLen+1];
	if (!buffer)
	{
		//fprintf(stderr, "Memory error!");
        fclose(file);
		return 0;
	}
	Data(buffer);
	//Read file contents into buffer
	fread(_data, fileLen, 1, file);
	fclose(file);

	return 1;
}
*/

void Turbo::Graphics::TurboBitmap::Draw(int width, int height, int style)
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

		if (actualWidth	!= _canvas->Width() ||
			actualHeight!= _canvas->Height() ||
			style		!= _style)
		{
			_canvas->Width(actualWidth);
			_canvas->Height(actualHeight);
			_style = style;

			if (actualWidth  == (int)(_bmpFile->info.width) &&
				actualHeight == (int)(_bmpFile->info.height))
				DrawBitmapSimple();
			else		
				DrawBitmap();
		}
	}
}

//  ============================================================================
//  Local Support Methods
//  ============================================================================

void Turbo::Graphics::TurboBitmap::DrawBitmapSimple()
{
	_canvas->Clear();

	byte *pixelData = ((byte*)_bmpFile) + _bmpFile->header.offset - 2;
	byte *firstPixel = pixelData;

	int pixelSize = _bmpFile->info.bitDepth / 8;
	int realWidth = (_bmpFile->info.width * pixelSize);
	if (realWidth % 4)
		realWidth += 4 - realWidth % 4;

	ITurboPen *pen = _canvas->Pen();

	for (unsigned int x = 0; x < _bmpFile->info.width;  x ++)
	for (unsigned int y = 0; y < _bmpFile->info.height; y++)
	{
		int yy = _bmpFile->info.height - 1 - y;

		byte *rgb = pixelData + yy * realWidth + x * pixelSize;

		if (pixelSize == 3)
		{
			int drawIt = 1;
			if ((_style & BMP_TRANSPARENT) &&
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

void Turbo::Graphics::TurboBitmap::DrawBitmap()
{
	int px0=0, py0=0, px1=0, py1=0;
	double xScale, yScale;
	int canvasWidth = _canvas->Width();
	int canvasHeight = _canvas->Height();

	_canvas->Clear();

	unsigned char *pixelData = ((unsigned char*)_bmpFile) + _bmpFile->header.offset - 2;
	int realWidth = (_bmpFile->info.width * 3);
	if (realWidth % 4)
		realWidth += 4 - realWidth % 4;

	ITurboPen *pen = _canvas->Pen();

	switch (_style & 3)
	{
		case BMP_NORMAL:
			xScale = 1.0;
			yScale = 1.0;
			if (_style & BMP_LEFT)
			{
				px0 = 0;
				px1 = _bmpFile->info.width - 1;
			}
			else if (_style & BMP_RIGHT)
			{
				px1 = (canvasWidth - 1);
				px0 = (canvasWidth - 1) - _bmpFile->info.width + 1;
			}
			else
			{
				px0 = (canvasWidth - _bmpFile->info.width) / 2;
				px1 = px0 + _bmpFile->info.width - 1;
			}
			if (_style & BMP_TOP)
			{
				py0 = 0;
				py1 = _bmpFile->info.height - 1;
			}
			else if (_style & BMP_BOTTOM)
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
		
		case BMP_ZOOM:
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
		
		case BMP_TILE:
			xScale = 1.0;
			yScale = 1.0;
			px0 = 0;
			px1 = (canvasWidth - 1);
			py0 = 0;
			py1 = (canvasHeight - 1);
			break;
		
		case BMP_STRETCH:
			xScale = _bmpFile->info.width  * 1.0 / canvasWidth;
			yScale = _bmpFile->info.height * 1.0 / canvasHeight;
			px0 = 0;
			px1 = (canvasWidth - 1);
			py0 = 0;
			py1 = (canvasHeight - 1);
			break;
	}

	
	for (int sx = px0; sx <= px1; sx ++)
	for (int sy = py0; sy <= py1; sy ++)
	{
		int ix = (int)((sx - px0) * xScale);
		int iy = (int)((sy - py0) * yScale);
		
		ix = ix % _bmpFile->info.width;
		iy = iy % _bmpFile->info.height;

		iy = _bmpFile->info.height - 1 - iy;
		
//		RGB24 rgb = RGBdata[ix + iy * _bmpFile->info.width];
		unsigned char *rgb = pixelData + ix * 3 + iy * realWidth;
		int drawIt = 1;
		if ((_style & BMP_TRANSPARENT) &&
			(rgb[0] == pixelData[0]) &&
			(rgb[1] == pixelData[1]) &&
			(rgb[2] == pixelData[2]))
			drawIt = 0;
			
		if (drawIt)
		{
			//u16 color = RGB15(rgb.red >> 3, rgb.green >> 3, rgb.blue >> 3);
			//_canvas->SetPixel(sx, sy, color);
//			pen->SetRGB(rgb.red, rgb.green, rgb.blue);
			pen->SetRGB(rgb[2], rgb[1], rgb[0]);
			_canvas->SetPixel(sx, sy, pen->GetColor());
		}
	}
}

