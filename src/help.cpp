#include <iostream>

#define HELP_TEXT ""\
  "Usage: todo [options] ['YOUR TODO TEXT']\n\n" \
  "todo without any parameters will simply print all of your current todo items\n\n"\
  "options:\n"\
  "-h\n"\
  "	Displays this help message\n"\
  "-i\n"\
  "	Displays to the user all the current lists and priorities with their colors and ids that \n"\
  "	available from within their '~/.todocfg' file. It also tells the users the files that their \n"\
  "	todo items are stored within\n"\
  "-l [list]\n"\
  "	If this parameter is assigned along with user input then a new todo item will be created\n"\
  "	within the specified list that is passed in as the flag parameter.\n"\
  "	If however there is not user input then the program will simply print out all todo items \n"\
  "	that exist within the specified list.\n"\
  "		EXAMPLES:\n"\
  "		todo -l 2 'Dont forget to water the plants'\n"\
  "		todo -l 2\n"\
  "-p [priority]\n"\
  "	If this parameter is assigned along with user input then a new todo item will be created\n"\
  "	with the specified priority level that is passed in as the flag parameter.\n"\
  "	If however there is not user input then the program will simply print out all todo items \n"\
  "	that exist with the specified priority level.\n"\
  "		EXAMPLES:\n"\
  "		todo -p 1 'Dont forget to water the plants'\n"\
  "		todo -p 2\n"\
  "-t [tag]\n"\
  "	If this parameter is assigned along with user input then a new todo item will be created\n"\
  "	with the specified tag string that is passed in as the flag parameter.\n"\
  "	If however there is not user input then the program will simply print out all todo items \n"\
  "	that exist with that specified tag string\n"\
  "		EXAMPLES:\n"\
  "		todo -t MONDAY 'Do this on Monday'\n"\
  "		todo -t 2\n"\
  "-c\n"\
  "	This flag will clear the screen and print the todo list with visual barriers around\n"\
  "	the entire output (This is useful for example in an automated tmux pane)\n"\
  "-r\n"\
  "	BE CAREFUL WITH THIS FLAG! If included this will reset all the todos (remove all todos) \n"\
  "	the program will ask you to confirm your choice before it carries out the action\n"\
  "-d [item id]\n"\
  "	Deletes the Todo item with the matching ID that is provided with the parameter\n"\
  "		EXAMPLE:\n"\
  "		todo -d 2\n"\
  "-e [item id]\n"\
  "	Edits the Todo item with the matching ID that is provided with the parameter. An interactive\n"\
  "	editing wizard will be started and the user will be prompted to edit the fields of the Todo Item.\n"\
  "	(Title, Tag, Priority, List) - Leaving any of these fields blank will leave un-changed.\n"\
  "		EXAMPLE:\n"\
  "		todo -e 2\n"\
  "-a\n"\
  "	If this flag is included the normal printing of all todos list will occur, however the backlog of\n"\
  "	of todo items will also be printed. This can be thought of as print ALL\n"\
  "		EXAMPLE:\n"\
  "		todo -a\n"\
  "-b [item id]\n"\
  "	Moves the Todo item with the matching id to the Backlog list (this can be viewed with the -a flag)\n"\
  "		EXAMPLE:\n"\
  "		todo -b 2\n"\
  "-m [item id]\n"\
  "	Moves the Todo item with the matching id from the Backlog to the main list of todo item\n"\
  "		EXAMPLE:\n"\
  "		todo -m 2\n"\
  ""

class Help {
    public:
        static void printHelp(){
            printf(HELP_TEXT);
        }
};
