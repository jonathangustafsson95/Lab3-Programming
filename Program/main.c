#include "simpio.h"
#include "sourcecode.h"


void main(void)
{
	DBT database;
	string filename;

	database = initializeDataBase();
	printIntroMessage();
	filename = getRegisterFilename();
	loadDataBase(filename, database);
	executeCommands(database);
	saveDataBase(filename, database);
}