//
//  VlwFont.cpp
//  vlw2h
//
//  Created by ThrudTheBarbarian on 5/29/22.
//

#include "VlwFont.h"
#include "debug.h"

namespace fs = std::filesystem;

/*****************************************************************************\
|* Constructor
\*****************************************************************************/
VlwFont::VlwFont(int debugLevel)
	    :_glyphCount(0)
	    ,_version(0)
	    ,_fontSize(0)
	    ,_ascent(0)
	    ,_descent(0)
	    ,_debugLevel(debugLevel)
	    ,_name("unnamed")
	{}
	
/*****************************************************************************\
|* Load a font from a file
\*****************************************************************************/
bool VlwFont::load(std::string filename)
	{
	bool ok = fs::exists(filename);
	if (ok)
		{
		FILE *fp = fopen(filename.c_str(), "rb");
		if (fp)
			{
			int32_t iVal;
			
			fread(&iVal, 4, 1, fp);
			_glyphCount = ntohl(iVal);

			fread(&iVal, 4, 1, fp);
			_version = ntohl(iVal);
			
			fread(&iVal, 4, 1, fp);
			_fontSize = ntohl(iVal);
			
			fread(&iVal, 4, 1, fp);
			fread(&iVal, 4, 1, fp);
			_ascent = ntohl(iVal);
			
			fread(&iVal, 4, 1, fp);
			_descent = ntohl(iVal);
			
			if (_debugLevel > 0)
				{
				printf(" - Font loaded\n"
					   "   - Glyphs          : %d\n"
					   "   - Version         : %d\n"
					   "   - Font size       : %d\n"
					   "   - Ascent          : %d\n"
					   "   - Descent         : %d\n",
					   _glyphCount, _version, _fontSize,
					   _ascent, _descent);
				}
			if (_version == 11)
				ok = _loadGlyphData(fp);
			else
				{
				LOG("Cannot parse header for font '%s'\n", filename.c_str());
				ok = false;
				}
			fclose(fp);
			}
		else
			{
			LOG("Cannot open font file '%s'\n", filename.c_str());
			ok = false;
			}
		}
		
	return ok;
	}


/*****************************************************************************\
|* Return a name that can be used by C++
\*****************************************************************************/
std::string VlwFont::saneName(void)
	{
	std::string name = "";
	for (char const &c: _name)
		if (isdigit(c) || isalpha(c) || c=='_')
			name += c;
	name += "_";
	name += std::to_string(_fontSize);
	
	return name;
	}
	
#pragma mark - Private Methods

/*****************************************************************************\
|* Private method - parse out the glyph data
\*****************************************************************************/
bool VlwFont::_loadGlyphData(FILE *fp)
	{
	bool ok = true;
	
	/*************************************************************************\
    |* First read the metadata
    \*************************************************************************/
	for (int i=0; i<_glyphCount && ok; i++)
		{
		VlwGlyph g(_debugLevel);
		ok = !g.parseMetadata(fp);
		_glyphs.push_back(g);
		}
	
	/*************************************************************************\
    |* Then the bitmap data
    \*************************************************************************/
	if (ok)
		{
		for (int i=0; i<_glyphCount && ok; i++)
			{
			VlwGlyph g = _glyphs[i];
			ok = g.parseBitmapdata(fp);
			if (ok)
				_glyphs[i] = g;
			}
		}
		
	/*************************************************************************\
    |* Variable length fields: name
    \*************************************************************************/
	if (ok)
		{
		uint8_t smooth 	= 0;
		uint16_t len	= 0;
		char str[256];
		
		fread(&len, 2, 1, fp);
		len = ntohs(len);
		fread(str, len+1, 1, fp);
		_name = str;
		if (_debugLevel > 0)
			printf("   - Font name       : '%s'\n", _name.c_str());
		
		fread(&len, 2, 1, fp);
		len = ntohs(len);
		fread(str, len+1, 1, fp);
		_psName = str;
		if (_debugLevel > 0)
			printf("   - Postscript name : '%s'\n", _name.c_str());
		
		fread(&smooth, 1, 1, fp);
		_smoothed = (len != 0);
		if (_debugLevel > 0)
			printf("   - Font %s anti-aliased \n",
				(_smoothed) ? ("is") : ("is not"));
		}
		
	return ok;
	}
	
