//
//  ArgParser.h
//  dbio
//
//  Created by ThrudTheBarbarian on 7/19/20.
//

#ifndef ArgParser_h
#define ArgParser_h

#include <cstdio>
#include <string>

#include "properties.h"
#include "macros.h"


class ArgParser
	{
	/***************************************************************************\
    |* Properties
    \***************************************************************************/
    GET(int, argc);                     // Copy of argc
    GET(const char **, argv);           // Copy of argv
    
    private:
        
    public:
        /***********************************************************************\
        |* Constructors and Destructor
        \***********************************************************************/
        ArgParser(int argc, const char **argv);

        /***********************************************************************\
        |* Access the fields
        \***********************************************************************/
        std::string stringFor(const char *shrt,
                              const char *lng,
                              const char *dflt);
                              
        int         intFor(const char *shrt, const char *lng, int dflt);
        float       floatFor(const char *shrt, const char *lng, float dflt);
        int         flagFor(const char *shrt, const char *lng, int dflt);

        
	};
	
#endif // ! ArgParser_h
