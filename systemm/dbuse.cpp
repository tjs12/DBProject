#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

#include "../common/rc.h"
#include "../storage/rmm.h"
#include "smm.h"

int main (
	int argc,		// length of array argv
	char **argv )	// argv is an array of pointers
					// argv[0] points to the name of the command
					// argv[1] points to argument DBname
{
	char *dbname;
	char command[80] = "cd";

	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " dbname \n";
		exit(1);
	}

	// The database name is the second argument
	dbname = argv[1];

	// Create a subdirectory for the database
	system (strcat(command,dbname));

	//...
	// initialize RedBase components
	//PF_Manager pfm;
	RM_Manager rmm(/*pfm*/);
	//IX_Manager ixm(pfm);
	SM_Manager smm(/*ixm, */rmm);
	//QL_Manager qlm(smm, ixm, rmm);
	
	// open the database
	RC rc;
	if (rc = smm.OpenDb(dbname))
		;//...
	// call the parser
	//RBparse(pfm, smm, qlm);
	// close the database
	if (rc = smm.CloseDb()) ;//...
}
