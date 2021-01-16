# Command line parser
Returns struct what contains divided command line to given options list and it`s arguments.
You can pass to function "const char *" line or already divided arguments what your programm receives by argc and argv parameters.

This functions is my own adaptation POSIX function "int getopt(int argc, char * const argv[], const char *optstring);" from unistd.h

Here is some rules according to what this functions work (basicly they are same as windows cmd or linux, only option template line have some modifications)
 - "-p" && "--path" is parameters; data what are after them name are arguments
 - if parameter has a value it ends with : example "p:"
 - if parameter have optional value (could be or not) it ends with two ::, example p::
 - long argument must be in the middle of two semicolons, example ;long_arg;
   and it can`t have spaces or other cpecial symbols only a-z, A-Z,0-9,
   example ;long_argment;
 - long arguments starts witn --, short arguments starts with -
 - parameter`s arguments what have spaces should be in quotes "", 
   example: --long_param "some multi word parameter with spaces", 
   because space is a delimiter to all parameters and them arguments

 Example: parameter (options) list is "abcd:e::;path;:"
 cmd line would be looks like that: some_command -a -b -c -d "optional argument" -e required_argument --path "optional argument to long parameter"

 There is two versions of cmd line parse functions

 getopt(int argc, char * const argv[],
		   const char *optstring);
This function takes already divided command line to parameters and arguments strings
   
 getopt(char * const argv,
		   const char *optstring);
And this one takes whole command and options string

For now there is VS project, after some tests and debugging i will add possibility to compile this code for more platforms.
Also i will add some function what will be processing cmd user input. You will have only write custom function according specific signature (to store them in array) 
and pass them in pair with related command. and all what you have to do will be to cast them form "char *" to specific types. 

P.S Sorry for any grammar mistakes, i am still learning english too. But i will be happy to know if i did any here especially if i did some in code what can cause undefined behaviour or something what can break all.

    