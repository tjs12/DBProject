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
      exit(1);
   }

   // The database name is the second argument
   dbname = argv[1];

   // Create a subdirectory for the database
   system (strcat(command,dbname));

   if (chdir(dbname) < 0) {
      cerr << argv[0] << " chdir error to " << dbname << "\n";
      exit(1);
   }

   // Create the system catalogs
   ...
}
