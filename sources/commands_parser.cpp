#include "commands_parser.h"
char * errorMessage[128] = { '\0' };

//to thack memory leaks

typedef struct PARAM_INFO
{
	struct OPTION_INFO
	{
		char *param;
		//char *arg;
		bool bIsLong;
		bool bHaveParameter;
		bool bParameterIsRequired;
	} option[MAX_OPTIONS_SIZE];
	/*char ** cmdArg;
	size_t size;*/
} *PPARAM_INFO;


void * customMalloc(size_t size)
{
	void * address = malloc(size);
	//printf("allocated %#010x\n", address);
	return address;
}

void customFree(void* pointer)
{
	//printf("deleted %#010x\n", pointer);
	free(pointer);
}

bool isParameter(char * var)
{
	if (!var)
		return false;
	if (var[0] == '-')
		return true;

	return false;
}

int findParam(char * line, PPARAM_INFO pInfo, size_t paramSize)
{
	for (size_t i = 0; i < paramSize; ++i)
	{
		if (!strcmp(pInfo->option[i].param, line))
			return i;
	}
	return -1; //not found
}

POPTIONS getOptions(const char *cmdLine, const char * options)
{
	size_t lineSize = strlen(cmdLine);

	char ** cmdArg = (char **)customMalloc(MAX_CMD_ARG * sizeof(char *));//[MAX_CMD_ARG];// = {0};
	memset(cmdArg, 0, MAX_CMD_ARG);

	size_t argLength = 0, argCount = 0;
	bool bSkipSpace = false;
	for (size_t i = 0; i < lineSize; ++i)
	{
		char asd = cmdLine[i];

		if (cmdLine[i] == '\"')
			bSkipSpace = !bSkipSpace;

		bool bisInTheEnd = i == lineSize - 1;
		if (!bSkipSpace && (cmdLine[i] == ' ' || bisInTheEnd))
		{
			if (bisInTheEnd)
				argLength++;

			cmdArg[argCount] = (char *)customMalloc(argLength + 1);
			memset(cmdArg[argCount], '\0', argLength + 1);
			memcpy(cmdArg[argCount], &cmdLine[i - argLength + (size_t)bisInTheEnd /*??some shit code??*/], argLength);
			argLength = 0;
			argCount++;
		}
		else
			argLength++;
	}
	
	POPTIONS opt = getOptions(argCount, cmdArg, options);

	// free
	for (size_t i = 0; i < argCount; i++)
		customFree(cmdArg[i]);
	customFree(cmdArg);

	return opt;
}

POPTIONS getOptions(size_t argc, char * const argv[], const char * options)
{
	PPARAM_INFO param = (PPARAM_INFO)customMalloc(sizeof(PARAM_INFO));
	size_t prCount = 0; // count of parsed options
	
	//parsing parameters (options)
	{
		size_t optionsLength = strlen(options);
		size_t optionSize = 0; //single option size, is for long options
		bool isLongParam = false;

		for (size_t i = 0; i < optionsLength; i++)
		{
			if (!isLongParam && options[i] != ';' && options[i] != ':')
			{
				param->option[prCount].bHaveParameter = false;
				param->option[prCount].bParameterIsRequired = false;

				param->option[prCount].bIsLong = false;

				param->option[prCount].param = (char *)customMalloc(sizeof(char) * 3);
				param->option[prCount].param[2] = '\0';
				param->option[prCount].param[0] = '-';
				param->option[prCount].param[1] = options[i];
				optionSize = 0;
				prCount++;
			}

			if (isLongParam && options[i] != ';')
				optionSize++;

			if (options[i] == ';')
			{
				if (isLongParam)
				{
					param->option[prCount].bHaveParameter = false;
					param->option[prCount].bParameterIsRequired = false;

					param->option[prCount].bIsLong = true;

					param->option[prCount].param = (char *)customMalloc(sizeof(char)*(optionSize + 1/*\0*/ + 2/*--*/));
					//memset(&opt->option[optSize].param[0], '\0', optionSize + 1/*\0*/ + 2/*--*/);
					param->option[prCount].param[optionSize + 2/*--*/] = '\0';
					memset(&param->option[prCount].param[0], '-', sizeof(char) * 2);
					memcpy(&param->option[prCount].param[2], &options[i - optionSize], optionSize);
					optionSize = 0;
					prCount++;
				}
				isLongParam = !isLongParam;
			}

			if (options[i] == ':')
			{
				if (!param->option[prCount - 1].bHaveParameter)
				{
					param->option[prCount - 1].bHaveParameter = true;
					param->option[prCount - 1].bParameterIsRequired = true;
				}
				else
				{
					param->option[prCount - 1].bParameterIsRequired = false;
					param->option[prCount - 1].bHaveParameter = true;
				}
			}
		}				
	}


	POPTIONS optOut = (POPTIONS)customMalloc(sizeof(OPTIONS));
	memset(optOut->options, 0, sizeof(OPTIONS::OPTION)* MAX_OPTIONS_SIZE);
	
	size_t optCount = 0;
	// parsing command line according to options
	{
		optOut->command = (char *)customMalloc(sizeof(char) * strlen(argv[0]) + 1);
		memcpy(optOut->command, argv[0], sizeof(char) * strlen(argv[0]) + 1);

		bool bNextMustBeArgument = false;
		bool bnextParameterIsRequired = false;

		for (size_t i = 1; i < argc; ++i)
		{
			if (bNextMustBeArgument)
			{
				int res = findParam(argv[i], param, prCount);
				if (bnextParameterIsRequired)
				{
					if (res == -1)
					{
						optOut->options[optCount - 1].argument = (char *)customMalloc(sizeof(char) * strlen(argv[i]) + 1);
						memcpy(optOut->options[optCount - 1].argument, argv[i], sizeof(char) * strlen(argv[i]) + 1);
					}
					else
						//optOut->options[optCount - 1].argument = NULL;
						printf("[ERROR] parameter %s requires argument\n", optOut->options[optCount - 1].option);
				}
				//else if (bnextParameterIsRequired || res == -1)
					//optOut->options[optCount - 1].argument = NULL;
					//printf("argument for %s is not required\n", optOut->options[optCount - 1].option);

				bNextMustBeArgument = false;
				bnextParameterIsRequired = false;
				continue;
			}
			//else
			//optOut->options[optCount - 1].argument = NULL;

			int res = findParam(argv[i], param, prCount);
			if (res != -1)
			{
				optOut->options[optCount].option = (char *)customMalloc(sizeof(char) * strlen(param->option[res].param) + 1);
				memcpy(optOut->options[optCount].option, param->option[res].param, sizeof(char) * strlen(param->option[res].param) + 1);

				bNextMustBeArgument = param->option[res].bHaveParameter;
				bnextParameterIsRequired = param->option[res].bParameterIsRequired;
				optCount++;

				//printf("this is option: %s\n", argv[i]);
			}
			//else
				//printf("this is NOT option: %s\n", argv[i]);
		}

		optOut->count = optCount;
	}

	//need to delete POPTIONS_INFO
	for (size_t i = 0; i < prCount; i++)
	{
		free(param->option[i].param);
	}
	free(param);

	return optOut;
}

void deleteOptionsStruct(POPTIONS options)
{
	customFree(options->command);

	for (size_t i = 0; i < options->count; i++)
	{
		if (options->options[i].argument)
			customFree(options->options[i].argument);

		if (options->options[i].option)
			customFree(options->options[i].option);

	}

	customFree(options);
}
