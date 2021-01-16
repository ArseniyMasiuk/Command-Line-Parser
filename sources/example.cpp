
#include "commands_parser.h"
#include "commands_parser.h"


int main()
{
	POPTIONS res = getOptions("some_command -a -b -c -d \"required argument\" -e \"optional argument\" --path \"required argument to long parameter\"", 
		"a:b::cd:e::;path;:");


	deleteOptionsStruct(res);

	return 0;
}




/**/