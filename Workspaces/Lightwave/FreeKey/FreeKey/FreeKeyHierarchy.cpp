//
// FreeKeyHierarchy.cpp
//

#include "FreeKeyHierarchy.h"

// BuildHierarchyFromScene():
//  This is a somewhat kludgy varient of the Batarang BuildHierarchyFromScene() function.
//   It calls Batarangs's function (thus allowing for only 1 function to be maintined),
//   and then promotes each BatarangHierarchy returned to a FreeKeyHierarchy, deleting
//   the original BatarangHierarhcy upon completion.
//  It's not pretty, but it works.
bool FreeKeyHierarchy::BuildHierarchyFromScene( ifstream &in, FreeKeyHierarchy &base,
                                                 DynArray<FreeKeyHierarchy *> &item_list ) {

}
