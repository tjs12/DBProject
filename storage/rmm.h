#ifndef RMM_H
#define RMM_H

#include "../common/rc.h"


class RM_FileHandle {
  public:
       RM_FileHandle  ();                                  // Constructor
       ~RM_FileHandle ();                                  // Destructor
    /*RC GetRec         (const RID &rid, RM_Record &rec) const;
                                                           // Get a record
    RC InsertRec      (const char *pData, RID &rid);       // Insert a new record,
                                                           //   return record id
    RC DeleteRec      (const RID &rid);                    // Delete a record
    RC UpdateRec      (const RM_Record &rec);              // Update a record
    RC ForcePages     (PageNum pageNum = ALL_PAGES) const; // Write dirty page(s)
      */                                                     //   to disk
};



class RM_Manager {
  public:
       RM_Manager  (/*PF_Manager &pfm*/);            // Constructor
       ~RM_Manager ();                           // Destructor
    RC CreateFile  (const char *fileName, int recordSize);
                                                 // Create a new file
    RC DestroyFile (const char *fileName);       // Destroy a file
    RC OpenFile    (const char *fileName, RM_FileHandle &fileHandle);
                                                 // Open a file
    RC CloseFile   (RM_FileHandle &fileHandle);  // Close a file
};

#endif
