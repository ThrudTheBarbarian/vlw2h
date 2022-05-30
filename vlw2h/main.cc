//
//  main.cpp
//  vlw2h
//
//  Created by ThrudTheBarbarian on 5/29/22.
//

#include <iostream>
#include "ArgParser.h"
#include "Converter.h"
#include "VlwFont.h"

/*****************************************************************************\
|* File globals
\*****************************************************************************/
static int _debugLevel = 1;

/*****************************************************************************\
|* Program entry
\*****************************************************************************/
int main(int argc, const char * argv[])
    {
    ArgParser args(argc, argv);
    
    _debugLevel = args.intFor("-d", "--debug-level", 1);
    
    std::string fontName = args.stringFor("-f", "--font", "");
    if (fontName.length() == 0)
        {
        LOG("Cannot open font file '%s'", fontName.c_str());
        exit(-1);
        }
        
    std::string header = args.stringFor("-h", "--header", "default.h");
    if (header == "default.h")
        WARN("No header file specified - using 'default.h");
    
    VlwFont font(_debugLevel);
    if (font.load(fontName))
		{
		Converter converter(_debugLevel);
		converter.setFont(&font);
		converter.convert(header);
		}
    
    return 0;
    }


/*****************************************************************************\
|* Required for debug.h, returns the current debug level
\*****************************************************************************/
int debugLevel(void)
    {
    return _debugLevel;
    }
