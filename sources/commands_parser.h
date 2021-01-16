#ifndef COMMAND_PARSER 
#define COMMAND_PARSER 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
some rules
#include <unistd.h>
int getopt(int argc, char * const argv[],
		   const char *optstring);

this function is my own adaptation to function above
 - "-p" && "--path" is parameters; data what are after them name is arguments
 - if parameter has a value it ends with : example p:
 - if parameter have optional value (could be or not) it ends with two ::, example p::
 - long argument must be in the middle oof two semicolons,
   and it cant have space or other cpecial symbols only a-z, A-Z,0-9,
   example ;long_argment;
 - long arguments starts witn --, short arguments starts with -
 - parameter`s arguments what have spaces should be in quotes "", 
   example: --long_param "some multi word parameter with spaces", 
   because space is a delimiter to all parameters and them arguments


 example: abcd:e::;path;:
 cmd line would be looks like that: some_command -a -b -c -d "required argument" -e "optional argument" --path "required argument to long parameter"

 there will be two versions of cmd line parse functions

 getopt(int argc, char * const argv[],
		   const char *optstring);
   this function takes already divided command line to parameters and arguments and options string
   
 getopt(char * const argv,
		   const char *optstring);
   and this one takes wide cmd and oprions string


   example of options line: abcd:e::;path;:
 cmd line would be looks like that: some_command -a -b -c -d "required argument" -e "optional argument" --path "required argument to long parameter"


*/

#define MAX_CMD_ARG 64
#define MAX_OPTIONS_SIZE 128
extern char * errorMessage[128];

typedef struct OPTIONS // parsed command line 
{
	char * command;
	size_t count;
	struct OPTION
	{
		char * option;
		char * argument; // NULL menas that option has no argument or its not required
	} options[MAX_OPTIONS_SIZE];
}*POPTIONS;

POPTIONS getOptions(const char *cmdLine, const char * options);

POPTIONS getOptions(size_t argc, char * const argv[], const char *options);

void deleteOptionsStruct(POPTIONS options);

#endif //  COMMAND_PARSER 

