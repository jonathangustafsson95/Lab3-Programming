/*
*  File: sourcecode.c
*  --------------------------------
*  Authors: Joel Askling, Jonathan Gustafsson
*  A source (c) file for the library header file intarr.h which constists of several functions which process a database of type DBT as a cat register.
*/
#include "sourcecode.h"

/*
* Constant: QUIT
* --------------
* The constant QUIT is to break the program
* from the main menu.
*/

#define QUIT -1

/*
* Constant: INITIAL_SIZE
* ----------------------
* The constant defines the initial
* size of the dynamic array holding
* the cat data.
*/

#define INITIAL_SIZE 6

/* Function prototypes */
void anyKey();
void printIntroMessage();
DBT initializeDataBase();
string getRegisterFilename();
DBT loadDataBase(string filename, DBT database);
void intitializeDoubleDataBase(DBT database);
DBT executeCommands(DBT database);
void saveDataBase(string filename, DBT database);
void displayCatMenu(void);
DBT addACat(DBT database);
bool alphabeticControl(string name);
DBT insertColours(DBT database, int currentID);
DBT updateACat(DBT database);
DBT removeACat(DBT database);
void listAllCats(DBT database);
void listAllCatsAndAttributes(DBT database);
void lookUpACat(DBT database);
int searchForACat(DBT database);
bool integerCheck(string userString);
void listStatistics(DBT database);
DBT sortCatRegister(DBT database);
DBT bubbleSortByName(DBT database);
DBT bubbleSortByBirthYear(DBT database);
//DBT correctID(DBT database);
string validCatName(void);
string validCatGender(void);
int validCatBirthyear(void);

/* Function: anyKey
*  Usage: anyKey();
*  --------------------------------
*  This function paus the calling function and let the user press any key to continue.
*/

void anyKey(void) 
{
	printf("Press any key to continue...");
	getchar();
}

/* Function: printIntroMessage
*  Usage: printIntroMessage();
*  --------------------------------
*  This function displays the intro message of the program.
*/

void printIntroMessage() {
	printf("This program helps organize information about cats\n");
	printf("and their attributes, list the names of cats currently\n");
	printf("in the organizer, list the cats and their attributes, look\n");
	printf("up the attributes of an individual cat, get statistics from the\n");
	printf("register, or sort the register.\n");
	anyKey();
}

/* Function: initializeDataBase
*  Usage: initializeDataBase();
*  --------------------------------
*  This function initializes a database using malloc on the struct DBT database and the struct catT as an dynamic array. It also sets
*  nextId = 1, currentNumberOfCats = 0 and currencMaxSize = INITIAL_SIZE in the DBT database struct. It returns the initialized database.
*/

DBT initializeDataBase()
{
	DBT database = malloc(sizeof(*database));
	database->cats = malloc(sizeof(catT)*INITIAL_SIZE);
	database->nextId = 1;
	database->currentNumberOfCats = 0;
	database->currentMaxSize = INITIAL_SIZE;
	return(database);
}

/* Function: getRegisterFilename
*  Usage: getRegisterFilename();
*  --------------------------------
*  This function asks the user for a filename and returns a string that will be used as an argument in the function getRegisterFilename.
*/

string getRegisterFilename()
{
	string filename;
	printf("\nEnter the name of the file that you want to edit: ");
	return(filename = GetLine());
}

/* Function: loadDataBase
*  Usage: loadDataBase(string filename, DBT database);
*  --------------------------------
*  This function takes a DBT database and a string filename as arguments, it opens a new or an existing file based on the filename it gets.
*  If then fills the database with data from the file if it exists using read-mode. Else it will open the file in write-mode.
*  The function returns a database, either empty or with data from the existing file.
*/

DBT loadDataBase(string filename, DBT database)
{
	FILE *infile = fopen(filename, "r");
	int count = 0, j = 0, i = 0;

	if (infile != NULL)
	{
		string line;

		while ((line = ReadLine(infile)) != NULL)
		{
			if (count == 0) {
				database->currentNumberOfCats = StringToInteger(line);
				if (database->currentNumberOfCats > INITIAL_SIZE) 
				{
					intitializeDoubleDataBase(database);
				}
			}
			if (count == 1) {
				database->cats[i].id = StringToInteger(line);
			}
			if (count == 2) {

				database->cats[i].name = line;
			}
			if (count == 3) {
				database->cats[i].gender = line[0];
			}
			if (count == 4) {
				database->cats[i].birthYear = StringToInteger(line);
			}
			if (count == 5) {
				database->cats[i].numberOfColours = StringToInteger(line);
			}

			int lengthColourArray = database->cats[i].numberOfColours;

			if (count > 5) {
				database->cats[i].colours[j] = line;
				j++;
			}
			if (count == lengthColourArray + 5) {
				j = 0;
				count = 0;
				if (i == database->currentNumberOfCats - 1) {
					database->nextId = database->currentNumberOfCats + 1;
				}
				i++;
			}
			count++;
		}
		fclose(infile);
	}
	else
	{
		infile = fopen(filename, "w");
		printf("Could'nt find a txt file with that name, creating a new file with the name '%s'\n", filename);
		getchar();
		return database;
	}
	return database;
}

/* Function: initializeDoubleDataBase
*  Usage: initializeDoubleDataBase(DBT database);
*  --------------------------------
*  This function takes a DBT database as an argument, it is triggered from functions as loadDataBase, addDataBase if the array of type catT is full.
*  It then initializes a new array of type catT called catsTmp, and allocates memory for the current size of the array times 2.
*  The data from the original catT array is copied to the new array. The old catT array allocated memory is freed up using free. And is replaced
*  by the new array of typ catT. DBT database currentMaxSize in the struct is updated with the new size of the array.
*/

void intitializeDoubleDataBase(DBT database) 
{
	int length = database->currentNumberOfCats;
	
	catT *catsTmp = (catT*)malloc(sizeof(catT)*length*2);

	for (int i = 0; i < length; i++)
	{
		catsTmp[i] = database->cats[i];
	}
	free(database->cats);
	database->currentMaxSize = length * 2;
	database->cats = catsTmp;
}

/* Function: executeCommands
*  Usage: executeCommands(DBT database);
*  --------------------------------
*  This function works as a bridge, it calls for every functions that either edit or displays data from the DBT database that it takes as an argument.
*  It asks the user which option the user wants, it displays them using the function displayCatMenu. It returns the database to the selected file.
*  If the user choose 0, then it will break the function and returns the database.
*/

DBT executeCommands(DBT database)
{
	bool run = TRUE;
	while (run) {
		printf("Choose an alternative..\n");
		displayCatMenu();
		int answer = GetInteger();
		switch (answer)
		{
		case 0:
			run = QUIT;
			return database;
		case 1:
			addACat(database);
			break;
		case 2:
			updateACat(database);
			break;
		case 3:
			removeACat(database);
			break;
		case 4:
			listAllCats(database);
			break;
		case 5:
			listAllCatsAndAttributes(database);
			break;
		case 6:
			lookUpACat(database);
			break;
		case 7:
			listStatistics(database);
			break;
		case 8:
			sortCatRegister(database);
			break;
		default:
			printf("invalid selection, please enter a number between 1-8. enter 0 to quit.\n");
			break;
		}
	}
}

/* Function: saveDataBase
*  Usage: saveDataBase(DBT database);
*  --------------------------------
*  This function takes a DBT database and a string filename as arguments, and prints the data from the DBT database (struct) to the filename.,
*  The file will be opened in write-mode and the data will be deleted in the file, and overwrited by the new data from the database.
*  It will then free up the memory allocated for the database, and the type catT dynamic array.
*/

void saveDataBase(string filename, DBT database)
{
	FILE *outfile;
	outfile = fopen(filename, "w");
	fprintf(outfile, "%d", database->currentNumberOfCats);
	for (int i = 0; i < database->currentNumberOfCats; i++) {
		fprintf(outfile, "\n%d", database->cats[i].id);
		fprintf(outfile, "\n%s", database->cats[i].name);
		fprintf(outfile, "\n%c", database->cats[i].gender);
		fprintf(outfile, "\n%d", database->cats[i].birthYear);
		fprintf(outfile, "\n%d", database->cats[i].numberOfColours);
		for (int j = 0; j < database->cats[i].numberOfColours; j++) {
			fprintf(outfile, "\n%s", database->cats[i].colours[j]);
		}
	}
	fprintf(outfile, "\n%d", database->nextId);
	fclose(outfile);
	realloc(database->cats, database->currentMaxSize);
	realloc(database, 0);
}

/* Function: displayCatMenu
*  Usage: displayCatMenu();
*  --------------------------------
*  This function displays all the options using the program to either edit or display data from the database.
*  The function is being used the the function executeCommands.
*/

void displayCatMenu(void) {
	printf("Enter an integer for your preferred choice:\n");
	printf("	Enter 1 to add a cat and its attributes. \n");
	printf("	Enter 2 to update an entry for a cat. \n");
	printf("	Enter 3 to remove a cat or its attributes. \n");
	printf("	Enter 4 to list all cats in the register. \n");
	printf("	Enter 5 to list all cats and their attributes. \n");
	printf("	Enter 6 to look up the attributes of a cat. \n");
	printf("	Enter 7 to list statistics from register. \n");
	printf("	Enter 8 to sort the register. \n");
	printf("	Enter 0 to Quit this program\n");
}

/* Function: addACat
*  Usage: addACat(DBT database);
*  --------------------------------
*  This function takes a DBT database as an argument and asks the user to put in data into the database, the database is being updated with the new
*  data from the user. Some data will automatically be updated in the database, such as numberOfColours, nextId, currentNumberOfCats and id.
*  It also checks if the array of type catT is full, if thats the case, then it calls for the function initializeDoubleDataBase, and expands the 
*  memory to the double size.
*  This function also has some control on the input from the user, it checks if the name only exists of alphabetic type character using the function alphabeticControl
*  It also checks if the gender input from the user is correct using controlCharacter.
*  It will use the function insertColours to insert the colours into the database.
*  It then returns the database.
*/

DBT addACat(DBT database)
{
	string userInputName, userInputGender;
	bool checkForAlphabetics, charControl;
	int userInputBirthYear;
	int currentID = database->nextId - 1;
	int currentDBColour = 0;

	if (database->currentNumberOfCats == database->currentMaxSize)
	{
		intitializeDoubleDataBase(database);
	}

	userInputName = validCatName();
	database->cats[currentID].name = userInputName;

	userInputGender = validCatGender();
	database->cats[currentID].gender = userInputGender[0];

	userInputBirthYear = validCatBirthyear();
	database->cats[currentID].birthYear = userInputBirthYear;

	database->cats[currentID].id = database->nextId;
	insertColours(database, currentID);
	database->currentNumberOfCats++;
	database->nextId++;
	return (database);
}

/* Function: alphabeticControl
*  Usage: alphabeticControl(string name);
*  --------------------------------
*  This function takes a string as an argument, and returns TRUE if the name only exists of alphabetic type of characters, else it returns FALSE.
*/

bool alphabeticControl(string name) 
{

	int length = StringLength(name), stringControl;

	for (int i = 0; i < length; i++)
	{
		stringControl = isalpha(name[i]);
		if (stringControl == 0) 
		{
			return FALSE;
		}
	}
	return TRUE;
}

/* Function: controlCharacter
*  Usage: controlCharacter(string gender);
*  --------------------------------
*  This function takes a string as an argument and returns TRUE if the string only exists of 1 element, and has the value of either
*  77 (M) or 70 (F), else it will return FALSE.
*/

bool controlCharacter(string gender) 
{
	int length = StringLength(gender);
	if (length > 1) 
		return FALSE;
	if (gender[0] == 77 || gender[0] == 70)
		return TRUE;
	else 
		return FALSE;
}

/* Function: insertColours
*  Usage: insertColours(DBT database, int currentID);
*  --------------------------------
*  This function lets the user input the colour that the user wants to register in the database, its arguments are DBT database and int currentID.
*  The currentID is necessery to register the colours on the right cat.
*  If the user just press enter (a empty string), then the function will break and return the database of type DBT.
*  There is also a control on the user input to check if the user has entered a string only with alphabetic type characters using the function alphabeticControl.
*  If the control returns FALSE then the function will ask the user for the colour again.
*/

DBT insertColours(DBT database, int currentID) 
{
	string userInputColour[MAX_COLOURS];
	int currentDBColour = 0;
	bool stringCompare, checkForAlphabetics;
	for (int i = 0; i < MAX_COLOURS; i++)
	{
		printf("\n%d. colour: ", i + 1);
		userInputColour[i] = ConvertToUpperCase(GetLine());
		stringCompare = StringEqual("", userInputColour[i]);
		checkForAlphabetics = alphabeticControl(userInputColour[i]);
		if (stringCompare == TRUE) {
			break;
		}
		if (checkForAlphabetics == FALSE) {
			printf("Invalid input, try again.\n");
			i--;
		}
		else {
			database->cats[currentID].colours[currentDBColour] = userInputColour[i];
			currentDBColour++;
		}
	}
	database->cats[currentID].numberOfColours = currentDBColour;
	return database;
}

/* Function: updateACat
*  Usage: updateACat(DBT database, int currentID);
*  --------------------------------
*  This function takes a DBT database as an argument and gets an int ID to edit the right cat from the function searchForACat.
*  It will then ask the user to update the name, birthYear and colours (using the function insertColours).
*  It then returns the updated database of type DBT.
*/

DBT updateACat(DBT database)
{
	string name;
	bool checkForAlphabetics;
	int birthYear;
	int ID = searchForACat(database) - 1;
	if (ID == -1) 
	{
		return database;
	}

	printf("\nUpdate name: "); 
	name = validCatName();
	database->cats[ID].name = name;
	printf("\nUpdate birth year: ");
	birthYear = validCatBirthyear();
	database->cats[ID].birthYear = birthYear;
	printf("\nUpdate colour/colours: ");
	insertColours(database, ID);
	return database;
}

/* Function: removeACat
*  Usage: removeACat(DBT database);
*  --------------------------------
*  This function takes a DBT database as an argument and gets an int ID to remove the right cat from the function searchForACat.
*  It then removes the cat from the DBT database and updates the id's for every cat in the register (database).
*  It also updates the currentNumberOfCats and the nextId in the DBT database, it returns the database.
*/

DBT removeACat(DBT database) 
 {
	 int ID = searchForACat(database) - 1;
	 if (ID == -1)
	 {
		 return database;
	 }

	 for (ID; ID < database->currentNumberOfCats; ID++)
	 {
		database->cats[ID] = database->cats[ID + 1];
		database->cats[ID].id = database->cats[ID].id - 1;
	 }

	 database->currentNumberOfCats--;
	 database->nextId--;
 }

/* Function: listAllCats
*  Usage: listAllCats(DBT database);
*  --------------------------------
*  This function takes a DBT database as an argument and displays all cats name and ID's in the register using a loop.
*/

void listAllCats(DBT database)
{
	int i;
	printf("All cats in the register: \n");
	for (i = 0; i < database->currentNumberOfCats; i++)
	{
		printf("%d. Cat name:%s \t Cat ID:%d \n", i + 1, database->cats[i].name, database->cats[i].id);
	}
	anyKey();
}

/* Function: listAllCatsAndAttributes
*  Usage: listAllCatsAndAttributes(DBT database);
*  --------------------------------
*  This function takes a DBT database as an argument and displays all cats and their attributes using a loop through the database.
*/

void listAllCatsAndAttributes(DBT database)
{
	int i = 0, j = 0, currentCatColor = 0;
	printf("All cats in the register & their attributes: \n");
	for (i = 0; i < database->currentNumberOfCats; i++)
	{
		printf("\n\nCat name: %s\n", database->cats[i].name);
		printf("Cat ID: %d\n", database->cats[i].id);
		printf("Cat Gender: %c\n", database->cats[i].gender);
		printf("Cat Birthyear: %d\n", database->cats[i].birthYear);
		printf("Cats Amount of colors: %d\n", database->cats[i].numberOfColours);
		for (j = 0; j < database->cats[i].numberOfColours; j++)
		{
			printf("Cats color: %s\n", database->cats[i].colours[j]);
		}
	}
	anyKey();
}

/* Function: loopUpACat
*  Usage: lookUpACat(DBT database);
*  --------------------------------
*  This function takes a database as an argument and are using the function searchForACat to get the right index of the cat that
*  the user wants to look up. It then displays the cat info from the database to the user.
*/

void lookUpACat(DBT database)
{
	int index = searchForACat(database) - 1;

	printf("Cat ID: %d\n", database->cats[index].id);
	printf("Cat name: %s\n", database->cats[index].name);
	printf("Cat Gender: %c\n", database->cats[index].gender);
	printf("Cat Birthyear: %d\n", database->cats[index].birthYear);
	printf("Cats Amount of colors: %d\n", database->cats[index].numberOfColours);
	printf("Colour/colours: \n");
	for (int j = 0; j < database->cats[index].numberOfColours; j++)
	{
		printf("%s\n", database->cats[index].colours[j]);
	}
	anyKey();
}

/*
* Function: searchForACat
* Usage: searchForACat(DBT database);
*-----------------------------------------
* This function recieves the database as input from executeCommands. The function then prompts the user to enter a name/id for a cat
* that they want to access. The input from the user is used to search the database for cats with the name/id of the users
* input. If there is more than one cat with the same name the user is prompted with the ID's and names of these cats.
* The user now have to choose one cat and that cat is returned as output. If the user searched for a cat by their ID the ID
* of that cat is returned as output. If the user input doesnt match any cat in the database the function return -1 as output.
*/

int searchForACat(DBT database) 
{
	string userChoice;
	bool stringAllow, integerStringCheck;
	int i = 0, stringInt;
	int countCats = 0;

	printf("Enter the name/ID of the cat you want to access: \n");
	userChoice = ConvertToUpperCase(GetLine());
	integerStringCheck = integerCheck(userChoice);

	if (integerStringCheck == TRUE) 
	{
		while (i != database->currentNumberOfCats) 
		{
			stringAllow = StringEqual(userChoice, database->cats[i].name);
			if (stringAllow == TRUE)
			{
				printf("%d. %s\n", i + 1, database->cats[i].name);
				countCats++;
			}
			i++;
		}
		if (countCats == 1) 
		{
			return i;
		}
		else if (countCats > 1)
		{
			printf("Which cat do you want to choose?");
			i = GetInteger();
			return i;
		}
	}
	else if (integerStringCheck == FALSE)
	{
		stringInt = StringToInteger(userChoice);
		while (i != database->currentNumberOfCats) 
		{
			if (stringInt == database->cats[i++].id)
			{
				return i;
			}
		}
	}
	printf("Your input did not match anything in the register...\n");
	anyKey();
	return -1;
}

/*
* Function: integerCheck
* Usage: integerCheck(string userstring);
*-----------------------------------------
* This function receives a string as input and check if the string contains any numbers.
* If it does contain any number, the function returns a boolean value (FALSE) as output.
* If not, it return the boolean value (TRUE).
*/

bool integerCheck(string userString)
{
	int i;
	int length = StringLength(userString);

	for (i = 0; i < length; i++)
	{
		if (userString[i] < '0' || userString[i] > '9')
		{
			return (TRUE);
		}
	}
	return FALSE;
}

/*
* Function: listStatistics
* Usage: listStatistics(DBT database);
*-----------------------------------------
* This function gets the database from executeCommands as input. The function lists
* the youngest and oldest cats in the register and also the amount of
* female and male cats in the register.
*/

void listStatistics(DBT database) 
{
	int youngest = database->cats[0].birthYear;
	int oldest = database->cats[0].birthYear;
	int genderAmountFemale = 0;
	int genderAmountMale = 0;

	printf("\nCurrent number of cats: %d", database->currentNumberOfCats);

	for (int i = 0; i < database->currentNumberOfCats; i++)
	{
		if (youngest < database->cats[i].birthYear) 
		{
			youngest = database->cats[i].birthYear;
		}
		else if (oldest > database->cats[i].birthYear) 
		{
			oldest = database->cats[i].birthYear;
		}

		if (database->cats[i].gender == 'F') 
		{
			genderAmountFemale++;
		}
		else if (database->cats[i].gender == 'M') 
		{
			genderAmountMale++;
		}
	}
	printf("\nThe youngest cats birth year is: %d", youngest);
	printf("\nThe oldest cats birth year is: %d", oldest);
	printf("\nThere is %d amounts of females and %d amounts of males.\n", genderAmountFemale, genderAmountMale);
	anyKey();
}

/*
* Function: sortCatRegister
* Usage: sortCatRegister(DBT database);
*-----------------------------------------
* Input to the function is the database received from executeCommands.
* This function promts the user with the choice of sorting the cats in alphabetic order by their names or
* in ascending order using their birthyear. After sorting the database by the users choice the database
* is sent as output back to executecommands.
* The default case breaks the switch and directs the user back to the main menu.
*/

DBT sortCatRegister(DBT database) 
{
	int userChoice;
	while (TRUE)
	{
		printf("\nPress 1 to sort by name");
		printf("\nPress 2 to sort by birth year\n: ");
		userChoice = GetInteger();

		switch (userChoice)
		{
		case 1:
			bubbleSortByName(database);
			return database;
		case 2:
			bubbleSortByBirthYear(database);
			return database;
		default:
			break;
		}
	}
}

/*
* Function: bubbleSortByName
* Usage: bubbleSortByName(DBT database);
*-----------------------------------------
* The function receives the database as input from sortCatRegister. The function sorts the cats in the database
* by their name in alphabetic order using the bubblesort algorithm.
* When the sorting is complete it returns the database as output with the cats sorted by name.
*/

DBT bubbleSortByName(DBT database) 
{
	catT tmp;
	for (int i = 1; i < database->currentNumberOfCats; i++)
	{
		for (int j = 0; j < database->currentNumberOfCats - i; j++)
		{
			if (database->cats[j].name[0] > database->cats[j + 1].name[0])
			{
				tmp = database->cats[j];
				database->cats[j] = database->cats[j + 1];
				database->cats[j + 1] = tmp;
			}
		}
	}
	printf("\nSorting by name is finnished.\n");
	anyKey();

	/*Use correctID if you want to set the id order sorted in the output file*/
	/*correctID(database);*/
	return database;
}

/*
* Function: bubbleSortByBirthYear
* Usage: bubbleSortByBirthYear(DBT database);
*-----------------------------------------
* The function receives the database as input from sortCatRegister.
* This function sorts the elements in the catT array by their birthyear in ascending order using the bubblesort algorithm.
* When the sorting is complete it returns the database as output with the cats sorted by birthyear.
*/

DBT bubbleSortByBirthYear(DBT database)
{
	catT tmp;
	for (int i = 1; i < database->currentNumberOfCats; i++)
	{
		for (int j = 0; j < database->currentNumberOfCats - i; j++)
		{
			if (database->cats[j].birthYear > database->cats[j + 1].birthYear)
			{
				tmp = database->cats[j];
				database->cats[j] = database->cats[j + 1];
				database->cats[j + 1] = tmp;
			}
		}
	}
	printf("\nSorting by birth year is finnished.\n");
	anyKey();

	/*Use correctID if you want to set the id order sorted in the output file*/
	/*correctID(database);*/
	return database;
}

/*
* Function: correctID
* Usage: correctID(DBT database);
*-----------------------------------------
* This function enters new ascending ID's for the cats after the user chosen to sort the cats.
*/

//DBT correctID(DBT database) 
//{
//	for (int i = 0; i < database->currentNumberOfCats; i++)
//	{
//		database->cats[i].id = i + 1;
//	}
//	return database;
//}

/*
* Function: validCatName
* Usage: validCatName();
*-----------------------------------------
* This function promts the user to enter a name for a cat. The input from the user is then checked if it contains
* only letters. If the input is valid its returned as ouput, otherwise the user is prompted to enter a new name.
*/

string validCatName(void)
{
	bool checkForAlphabetics;
	string userInputName;
	printf("\nEnter the name of the cat: ");
	userInputName = ConvertToUpperCase(GetLine());
	checkForAlphabetics = alphabeticControl(userInputName);
	if (checkForAlphabetics == FALSE)
	{
		printf("The name for a cat must only contain letters, try again..\n");
		validCatName();
	}
	else
	{
		return userInputName;
	}
}

/*
* Function: validCatGender
* Usage: validCatGender();
*-----------------------------------------
* This function promts the user to enter a gender for a cat. The input from the user is then checked if it contains
* only one char (F/M). If the input is valid its returned as output, otherwise the user is prompted to enter a new gender.
*/

string validCatGender(void)
{
	bool charControl;
	string userInputGender;
	printf("\nEnter the gender of the cat: ");
	userInputGender = ConvertToUpperCase(GetLine());
	charControl = controlCharacter(userInputGender);
	if (charControl == FALSE)
	{
		printf("The gender of a cat must be F/M, try again..\n");
		validCatGender();
	}
	else
	{
		return userInputGender;
	}
}

/*
* Function: validCatBirthYear
* Usage: validCatBirthYear();
*-----------------------------------------
* This function promts the user to enter a birthyear for a cat. The function calls GetInteger to check if the user entered an integer.
* If not, the user will be forced to retry until an integer is entered. When an integer is entered the function returns
* that integer as output.
*/

int validCatBirthyear(void)
{
	int userInputBirthYear;

	printf("\nEnter birth year of the cat: ");
	userInputBirthYear = GetInteger();
	return userInputBirthYear;
}