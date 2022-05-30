//
//  VlwGlyph.cpp
//  vlw2h
//
//  Created by ThrudTheBarbarian on 5/29/22.
//

#include "VlwGlyph.h"
#include "debug.h"
#include "macros.h"

/*****************************************************************************\
|* Constructor
\*****************************************************************************/
VlwGlyph::VlwGlyph(int debugLevel)
		 :_unicode(0)
		 ,_height(0)
		 ,_width(0)
		 ,_advance(0)
		 ,_dY(0)
		 ,_dX(0)
		 ,_padding(0)
		 ,_bitmap(nullptr)
		 ,_debugLevel(debugLevel)
	{}
	
/*****************************************************************************\
|* Parse metadata from a file stream
\*****************************************************************************/
bool VlwGlyph::parseMetadata(FILE *fp)
	{
	int32_t iVal;
	
	fread(&iVal, 4, 1, fp);
	_unicode = ntohl(iVal);
	
	fread(&iVal, 4, 1, fp);
	_height = ntohl(iVal);
	
	fread(&iVal, 4, 1, fp);
	_width = ntohl(iVal);
	
	fread(&iVal, 4, 1, fp);
	_advance = ntohl(iVal);
	
	fread(&iVal, 4, 1, fp);
	_dY = ntohl(iVal);
	
	fread(&iVal, 4, 1, fp);
	_dX = ntohl(iVal);
	
	fread(&iVal, 4, 1, fp);
	_padding = ntohl(iVal);

	if (_debugLevel > 2)
		printf("   - [%04x] '%c' h:%d w:%d a:%d dx:%d dy:%d p:%d\n",
			_unicode,
			((_unicode >= ' ' && _unicode <='z') ? (uint8_t)_unicode : '-'),
			_height, _width, _advance, _dX, _dY, _padding);
			
	return feof(fp);
	}

/*****************************************************************************\
|* Parse the bitmap for a glyph from a file stream
\*****************************************************************************/
bool VlwGlyph::parseBitmapdata(FILE *fp)
	{
	bool ok = true;
	
	_bitmap 	= new uint8_t [_width * _height];
	int bytes	= (int) fread(_bitmap, _width, _height, fp);
	if (bytes != _height)
		ok = false;
	
	return ok;
	}
