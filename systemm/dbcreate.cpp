#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main (
 int argc,      // length of array argv
 char **argv )  // argv is an array of pointers
                // argv[0] points to the name of the command
                // argv[1] points to argument DBname
{
   char *dbname;
   char command[80] = "mkdir ";

   if (argc != 2) {
      cerr << "Usage: " << argv[0] << " dbname \n";
      return 0;
   }

   // The database name is the second argument
   dbname = argv[1];

   // Create a subdirectory for the database
   system (strcat(command,dbname));

   if (chdir(dbname) < 0) {
      cerr << argv[0] << " chdir error to " << dbname << "\n";
      return 0;
   }

   // Create the system catalogs
   //...
}
