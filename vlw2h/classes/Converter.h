//
//  Converter.h
//  vlw2h
//
//  Created by ThrudTheBarbarian on 5/29/22.
//

#ifndef Converter_h
#define Converter_h

#include <stdio.h>
#include "properties.h"

class VlwFont;

class Converter
    {
	/*************************************************************************\
    |* Properties
    \*************************************************************************/
	GETSET(VlwFont *, font, Font);			// Font to convert
	GET(int, debugLevel);					// Debugging level
	
    public:
        /*********************************************************************\
        |* Constructors and Destructor
        \*********************************************************************/
         explicit Converter(int debugLevel = 0);
		
        /*********************************************************************\
        |* Write data to the output header file
        \*********************************************************************/
		bool convert(std::string filename);

    private:
		
        /*********************************************************************\
        |* Dump out header info to the passed file
        \*********************************************************************/
        void _header(FILE *fp, std::string name);
        void _heights(FILE *fp, std::string name);
        void _widths(FILE *fp, std::string name);
        void _advance(FILE *fp, std::string name);
        void _footer(FILE *fp, std::string name);
        
        void _dump8(FILE *fp, std::string name, std::string type, uint8_t *data);
    };

#endif /* Converter_h */
