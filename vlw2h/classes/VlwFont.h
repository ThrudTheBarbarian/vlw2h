//
//  VlwFont.h
//  vlw2h
//
//  Created by ThrudTheBarbarian on 5/29/22.
//

#ifndef VlwFont_h
#define VlwFont_h

/*****************************************************************************\
|* The vlw font format does not appear to be documented anywhere, so some
|*   reverse engineering has been applied!
|*
|*    Note: All multi-byte values are in network order
|*
|*    Header of vlw file comprises 6 uint32_t parameters (24 bytes total):
|*      1. The gCount (number of character glyphs)
|*      2. A version number (0xB = 11 for the one I am using)
|*      3. The font size (in points, not pixels)
|*      4. Deprecated mboxY parameter (typically set to 0)
|*      5. Ascent in pixels from baseline to top of "d"
|*      6. Descent in pixels from baseline to bottom of "p"
|*
|*    Next are gCount sets of values for each glyph, each set comprises 7
|*    int32t parameters (28 bytes):
|*      1. Glyph Unicode stored as a 32 bit value
|*      2. Height of bitmap bounding box
|*      3. Width of bitmap bounding box
|*      4. gxAdvance for cursor (setWidth in Processing)
|*      5. dY = distance from cursor baseline to top of glyph bitmap (signed
|*		   value +ve = up)
|*      6. dX = distance from cursor to left side of glyph bitmap (signed
|*		   value -ve = left)
|*      7. padding value, typically 0
|*
|*    The bitmaps start next at 24 + (28 * gCount) bytes from the start of the
|*    file. Each pixel is 1 byte, an 8 bit Alpha value which represents the
|*    transparency from 0xFF foreground colour, 0x00 background. The library
|*    uses a linear interpolation between the foreground and background RGB
|*    component colours. e.g.
|*			pixelRed = ((fgRed * alpha) + (bgRed * (255 - alpha))/255
|*
|*    To gain a performance advantage fixed point arithmetic is used with
|*    rounding and division by 256 (shift right 8 bits is faster).
|*
|*    After the bitmaps is:
|*       2 bytes for font name string length
|*       a non-terminated character string giving the font name
|*       2 bytes for Postscript name string length
|*       a non-terminated character string giving the font name
|*       last byte is 0 for non-anti-aliased and 1 for anti-aliased (smoothed)
|*
|*
|*    Glyph bitmap example is:
|*
|*       Cursor coordinate positions for this, next character are marked by 'C'
|*
|*       C<------- gxAdvance ------->C   o gxAdvance is how far to move cursor
|*       |                           |     for next glyph cursor position
|*       |                           |   o ascent is top of "d",
|*       +-- gdX --+             ascent  o descent is bottom of "p"
|*       |         +-- gWidth--+     |   o gdX is offset to left edge of bitmap
|*       |   +     x@.........@x  +  |   o gWidth is glyph bitmap width
|*       |   |     @@.........@@  |  |   o gHeight is glyph bitmap height
|*       |   |     @@.........@@ gdY |   o gdY is offset to top edge of bitmap
|*       |   |     .@@@.....@@@@  |  |
|*       | gHeight ....@@@@@..@@  +  +    <-- baseline
|*       |   |     ...........@@     |
|*       |   |     ...........@@     |
|*       |   |     .@@.......@@. descent
|*       |   +     x..@@@@@@@..x     |   x marks the corner pixels of bitmap
|*       |                           |
|*       +---------------------------+
|*
|*
|*	o gdX may be negative e.g. italic "y" tail extending to left of cursor
|*	  position, plot top left corner of bitmap at (cursorX + gdX)
|*	o plot top edge of bitmap at (cursorY + ascent - gdY)
|*	o yAdvance is y delta for the next line, font size or (ascent + descent)
|*	o some fonts can overlay in y direction so may need a user adjust value
|*
\*****************************************************************************/

#include <ctype.h>

#include <cstdio>
#include <string>
#include <filesystem>
#include <vector>

#include "properties.h"
#include "VlwGlyph.h"

class VlwFont
    {
	/*************************************************************************\
    |* Properties
    \*************************************************************************/
    GET(int, glyphCount);					// Number of glyphs
    GET(int, version);						// Version of the file
    GET(int, fontSize);						// Font size in points
    GET(int, ascent);						// Ascent as above
    GET(int, descent);						// Descent as above
	GET(int, debugLevel);					// How chatty to be
	GET(std::string, name);					// Font name
	GET(std::string, psName);				// Postscript Font name
	GET(bool, smoothed);					// If anti-aliased
	GET(std::vector<VlwGlyph>, glyphs);
		
    public:
        /*********************************************************************\
        |* Constructors and Destructor
        \*********************************************************************/
        explicit VlwFont(int debugLevel);

        /*********************************************************************\
        |* Initialise the font from a file
        \*********************************************************************/
		bool load(std::string filename);
	
        /*********************************************************************\
        |* Return a name that can be used by C++
        \*********************************************************************/
		std::string saneName(void);
		
    private:
        /*********************************************************************\
        |* Private method - parse out the glyph data
        \*********************************************************************/
        bool _loadGlyphData(FILE *fp);
    };


#endif /* VlwFont_h */
