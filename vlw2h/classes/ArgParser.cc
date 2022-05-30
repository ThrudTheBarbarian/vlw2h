//
//  ArgParser.cc
//  dbio
//
//  Created by ThrudTheBarbarian on 7/19/20.
//

#include "ArgParser.h"

/*****************************************************************************\
|* Store the initial values for argc, argv
\*****************************************************************************/
ArgParser::ArgParser(int argc, const char **argv)
          :_argc(argc)
          ,_argv(argv)
    {}
    
/*****************************************************************************\
|* Search for a string argument
\*****************************************************************************/
std::string ArgParser::stringFor(const char *shrt,
                                 const char *lng,
                                 const char *dflt)
	{
	for (int i=1; i<_argc-1; i++)
		if (strcmp(shrt, _argv[i]) == 0 || strcmp(lng, _argv[i]) == 0)
			return std::string(_argv[i+1]);
	return std::string(dflt);
	}
    
/*****************************************************************************\
|* Search for an int argument
\*****************************************************************************/
int ArgParser::intFor(const char *shrt, const char *lng, int dflt)
	{
	for (int i=1; i<_argc-1; i++)
		if (strcmp(shrt, _argv[i]) == 0 || strcmp(lng, _argv[i]) == 0)
			return atoi(_argv[i+1]);
	return dflt;
	}
    
/*****************************************************************************\
|* Search for flag arguments. Allow multiple
\*****************************************************************************/
int ArgParser::flagFor(const char *shrt, const char *lng, int dflt)
	{
    int total = 0;
	for (int i=1; i<_argc; i++)
		if (strcmp(shrt, _argv[i]) == 0 || strcmp(lng, _argv[i]) == 0)
			total ++;

	return (total == 0) ? dflt : total;
	}
    
/*****************************************************************************\
|* Search for a float argument
\*****************************************************************************/
float ArgParser::floatFor(const char *shrt, const char *lng, float dflt)
	{
	for (int i=1; i<_argc-1; i++)
		if (strcmp(shrt, _argv[i]) == 0 || strcmp(lng, _argv[i]) == 0)
			return atof(_argv[i+1]);
	return dflt;
	}
