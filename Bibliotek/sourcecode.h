/*
*  File: source.h
*  --------------------------------
*  Authors: Joel Askling, Jonathan Gustafsson
*  Task: A library header file for c whichconstists of several functions which process a database of type DBT as a cat register.
*/

#ifndef _sourcecode_h
#define _sourcecode_h

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

/* Constant: MAX_COLOURS
* ---------------------
* The constant defines the maximum number
* of colours that one cat can have.
*/

#define MAX_COLOURS 5

/* Type definition: catT
* ---------------------
* The catT is a simple struct containing all the
* attributes of the cats. The colours attribute
* is an array, since a cat can have several colours.
*/

typedef struct {
	int id;
	string name;
	char gender;
	int birthYear;
	int numberOfColours;
	string colours[MAX_COLOURS];
} catT;

/* Type definition: DBT
* --------------------
* The DBT type is a pointer to a struct containing a dynamic
* array of catT and three integer counters: one to keep track of the
* current size of the catT vector, one to keep track of the
* current number of cats in the database and another to keep track
* of the next available id number.
*/

typedef struct {
	catT *cats;
	int currentMaxSize;
	int currentNumberOfCats;
	int nextId;
} *DBT;

/* Function: initializeDataBase
*  Usage: initializeDataBase();
*  --------------------------------
*  This function initializes a database of type DBT and allocate memory for it.
*/

DBT initializeDataBase();

/* Function: getRegisterFilename
*  Usage: getRegisterFilename();
*  --------------------------------
*  This function asks the user for a filename and returns a string.
*/

string getRegisterFilename();

/* Function: initializeDoubleDataBase
*  Usage: initializeDoubleDataBase(DBT database);
*  --------------------------------
*  This function loads a database, its arguments is a string (filename) and a DBT type (database). If the file doesn't exists, then it will create a
*  new file.
*/

DBT loadDataBase(string filename, DBT database);

/* Function: executeCommands
*  Usage: executeCommands(DBT database);
*  --------------------------------
*  This function works as a bridge, it calls for every functions that either edit or displays data from the DBT database that it takes as an argument.
*  It asks the user which option the user wants, it displays them using the function displayCatMenu. It returns the database to the selected file.
*  If the user choose 0, then it will break the function and returns the database.
*/

DBT executeCommands(DBT database);

/* Function: saveDataBase
*  Usage: saveDataBase(DBT database);
*  --------------------------------
*  This function takes a DBT database and a string filename as arguments, and prints the data from the DBT database (struct) to the filename.,
*  The file will be opened in write-mode and the data will be deleted in the file, and overwrited by the new data from the database.
*  It will then free up the memory allocated for the database, and the type catT dynamic array.
*/

void saveDataBase(string filename, DBT database);

#endif /* _sourcecode_h */