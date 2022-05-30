//
//  Converter.cc
//  vlw2h
//
//  Created by ThrudTheBarbarian on 5/29/22.
//

#include "Converter.h"
#include "VlwFont.h"
#include "VlwGlyph.h"
#include "debug.h"

#include <sys/time.h>
#include <time.h>

/*****************************************************************************\
|* Constructor
\*****************************************************************************/
Converter::Converter(int debugLevel)
		  :_font(nullptr)
		  ,_debugLevel(debugLevel)
	{}
	
/*****************************************************************************\
|* Convert the font to a header-style format
\*****************************************************************************/
bool Converter::convert(std::string filename)
	{
	if (_font == nullptr)
		{
		LOG("No font to convert\n");
		return false;
		}
		
	FILE *fp = fopen(filename.c_str(), "wb");
	if (fp == NULL)
		{
		LOG("Cannot open output file '%s'", filename.c_str());
		return false;
		}
	
	std::string name = _font->saneName();
	
	_header(fp, name);
	_heights(fp, name);
	_widths(fp, name);
	_advance(fp, name);
	_footer(fp, name);
	
	fclose(fp);
	return true;
	}

	
/*****************************************************************************\
|* Private method: dump a header to the file
\*****************************************************************************/
void Converter::_header(FILE *fp, std::string name)
	{
    char cron[80];
	time_t now 			= time(NULL);
    struct tm tstruct 	= *localtime(&now);
    strftime(cron, sizeof(cron), "%Y-%m-%d %X", &tstruct);
    
    const char * dname	= name.c_str();
    
	fprintf(fp,
	"// Header-ised font '%s'\n"
	"//\n"
	"// Do not edit: auto-generated file!\n"
	"// Created at %s\n"
	"//\n"
	"// Stats:\n"
	"//   - Glyphs          : %d\n"
	"//   - Font version    : %d\n"
	"//   - Font size       : %d\n"
	"//   - Font ascent     : %d\n"
	"//   - Font descent    : %d\n"
	"//   - Anti-aliased    : %s\n\n\n"
	"#ifndef __font_header_%s__\n"
	"#define __font_header_%s__\n\n"
	"// Font defines\n"
	"#define glyph_%s      (%d)\n"
	"#define version_%s    (%d)\n"
	"#define fontSize_%s   (%d)\n"
	"#define ascent_%s     (%d)\n"
	"#define descent_%s    (%d)\n"
	"#define smoothed_%s   (%d)\n"
	"#define name_%s       \"%s\"\n\n"
	,dname, cron,
	_font->glyphCount(),
	_font->version(),
	_font->fontSize(),
	_font->ascent(),
	_font->descent(),
	_font->smoothed() ? ("yes") : ("no")
	,dname, dname
	,dname, _font->glyphCount()
	,dname, _font->version()
	,dname, _font->fontSize()
	,dname, _font->ascent()
	,dname, _font->descent()
	,dname, _font->smoothed()
	,dname, dname
	);
	}

	
/*****************************************************************************\
|* Private method: dump a header to the file
\*****************************************************************************/
void Converter::_heights(FILE *fp, std::string name)
	{
	uint8_t data[_font->glyphCount()];
	
	int count = 0;
	for (VlwGlyph g : _font->glyphs())
		data[count++] = g.height();
		
	_dump8(fp, name, "heights", data);
	}
	
/*****************************************************************************\
|* Private method: dump a header to the file
\*****************************************************************************/
void Converter::_widths(FILE *fp, std::string name)
	{
	uint8_t data[_font->glyphCount()];
	
	int count = 0;
	for (VlwGlyph g : _font->glyphs())
		data[count++] = g.width();
		
	_dump8(fp, name, "widths", data);
	}
	
/*****************************************************************************\
|* Private method: dump a header to the file
\*****************************************************************************/
void Converter::_advance(FILE *fp, std::string name)
	{
	uint8_t data[_font->glyphCount()];
	
	int count = 0;
	for (VlwGlyph g : _font->glyphs())
		data[count++] = g.advance();
		
	_dump8(fp, name, "advance", data);
	}

	
/*****************************************************************************\
|* Private method: dump data that fits into 8-bits into the file
\*****************************************************************************/
void Converter::_dump8(FILE *fp, std::string name, std::string type, uint8_t *data)
	{
    const char * dname	= name.c_str();
	fprintf(fp, "const uint8_t %s_%s[%d] =\n              {\n/* 0x0000 */ ",
			type.c_str(), dname, _font->glyphCount());
	
	bool newRow 		= true;
	const char *comma 	= " ";
	int num 			= _font->glyphCount();
	int count			= 0;
	
	for (int i=0; i<num; i++)
		{
		newRow = false;
		fprintf(fp, "%s0x%02x", comma, data[i]);
		comma = ",";
		
		count ++;
		if (count %12 == 0)
			{
			newRow = true;
			if (count < num)
				fprintf(fp, "\n/* 0x%04x */ ", count);
			else
				fprintf(fp, "\n             ");
			}
		}
		
	if (!newRow)
		fprintf(fp, "\n             ");
	fprintf(fp, "};\n\n");
	}

/*****************************************************************************\
|* Private method: dump a footer to the file
\*****************************************************************************/
void Converter::_footer(FILE *fp, std::string name)
	{
	fprintf(fp, "\n\n#endif // !__font_header_%s__\n\n\n", name.c_str() );
	}
	
