#ifndef SMM_H
#define SMM_H

// Used by SM_Manager::CreateTable
struct AttrInfo {
   char     *attrName;           // Attribute name
   AttrType attrType;            // Type of attribute
   int      attrLength;          // Length of attribute
};

// Used by Printer class
struct DataAttrInfo {
   char     relName[MAXNAME+1];  // Relation name
   char     attrName[MAXNAME+1]; // Attribute name
   int      offset;              // Offset of attribute 
   AttrType attrType;            // Type of attribute 
   int      attrLength;          // Length of attribute
   int      indexNo;             // Attribute index number
};


class SM_Manager {
  public:
       SM_Manager  (IX_Manager &ixm, RM_Manager &rmm);  // Constructor
       ~SM_Manager ();                                  // Destructor
    RC OpenDb      (const char *dbName);                // Open database
    RC CloseDb     ();                                  // Close database
    RC CreateTable (const char *relName,                // Create relation
                    int        attrCount,
                    AttrInfo   *attributes);
    RC DropTable   (const char *relName);               // Destroy relation
    RC CreateIndex (const char *relName,                // Create index
                    const char *attrName);
    RC DropIndex   (const char *relName,                // Destroy index
                    const char *attrName);
    RC Load        (const char *relName,                // Load utility
                    const char *fileName);
    RC Help        ();                                  // Help for database
    RC Help        (const char *relName);               // Help for relation
    RC Print       (const char *relName);               // Print relation
    RC Set         (const char *paramName,              // Set system parameter
                    const char *value);
};

class Printer {
  public: 
    Printer(const DataAttrInfo *attributes, const int attrCount);
    ~Printer();         
    void PrintHeader(ostream &c) const;
    Void Print(ostream &c, const char * const data);
    void Print(ostream &c, const void * const data[]);
    void PrintFooter(ostream &c) const;
};

#endif