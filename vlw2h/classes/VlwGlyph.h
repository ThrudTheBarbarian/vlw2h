//
//  VlwGlyph.h
//  vlw2h
//
//  Created by ThrudTheBarbarian on 5/29/22.
//

#ifndef VlwGlyph_h
#define VlwGlyph_h

#include <stdio.h>

#include "properties.h"

class VlwGlyph
    {
	/*************************************************************************\
    |* Properties
    \*************************************************************************/
    GETSET(uint32_t, unicode, Unicode);	// Glyph Unicode stored as 32 bit value
    GETSET(int, height, Height);		// Height of bitmap bounding box
    GETSET(int, width, Width);			// Width of bitmap bounding box
    GETSET(int, advance, Advance);		// gxAdvance for cursor
    GETSET(int, dY, DY);				// baseline to top (signed, +ve = up)
    GETSET(int, dX, DX);				// cursor to left (signed, -ve = left)
    GETSET(int, padding, padding);		// padding value, typically 0
	GETSET(uint8_t *, bitmap, Bitmap);	// alpha data per pixel
	GET(int, debugLevel);				// Debug level
	
    public:
        /*********************************************************************\
        |* Constructors and Destructor
        \*********************************************************************/
         explicit VlwGlyph(int debugLevel = 0);
		
        /*********************************************************************\
        |* Read metadata about a glyph from the file stream
        \*********************************************************************/
		bool parseMetadata(FILE *fp);

        /*********************************************************************\
        |* Read bitmap data about a glyph from the file stream
        \*********************************************************************/
		bool parseBitmapdata(FILE *fp);

    private:
    };


#endif /* VlwGlyph_h */
