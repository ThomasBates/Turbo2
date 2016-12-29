//  ========================================================================  //
//  DSBitmap.h
//  ========================================================================  //

#pragma once

#include "IImage.h"

typedef enum {
	BMP_NORMAL		= 0x0000,
	BMP_ZOOM		= 0x0001,
	BMP_TILE		= 0x0002,
	BMP_STRETCH		= 0x0003,
	
	BMP_CENTER		= 0x0000,
	BMP_LEFT		= 0x0010,
	BMP_RIGHT		= 0x0020,
	BMP_TOP			= 0x0040,
	BMP_BOTTOM		= 0x0080,
	
	BMP_TRANSPARENT	= 0x0100,
	BMP_ALPHA		= 0x0200
} BMP_DRAW_STYLE;

typedef struct
{
	char signature[2];
} BmpSignature;

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
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char alpha;
} RGB32;

typedef struct
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} RGB24;

typedef struct
{
	BmpHeader		header;
	BmpImageInfo	info;
//	RgbPalette		*palette;
//	unsigned char	*image8;
//	unsigned short	*image16;
//	RGB24			*imageRGB;
} BmpFile;

class Bitmap: public IImage
{
private:
	char*		_data;
	BmpFile*	_bmpFile;
	ICanvas*	_canvas;
	int			_width;
	int			_height;
	int			_style;

	void	DrawBitmapSimple();
	void	DrawBitmap();

public:

	//  Constructors
	Bitmap(ICanvas *canvas = 0, void *data = 0);
	~Bitmap();

	//  Property Accessor Methods
	virtual	void*	GetData() { return (void*)_data; }
	virtual void	SetData(void* data);
	virtual	ICanvas*GetCanvas() { return _canvas; }
	virtual void	SetCanvas(ICanvas *canvas) { _canvas = canvas; }

	virtual	int		GetWidth() { return _width; }
	virtual	void	SetWidth(int width) { _width = width; }
	virtual int		GetHeight() { return _height; }
	virtual void	Setheight(int height) { _height = height; }
	virtual	int		GetStyle() { return _style; }
	virtual	void	SetStyle(int style) { _style = style; }

	//  Methods
	virtual int		LoadFromFile(std::string fileName);
	virtual void	Draw(int width = 0, int height = 0, int style = 0);
};

