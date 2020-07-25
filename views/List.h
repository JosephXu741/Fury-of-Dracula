#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include "Trap.h"

typedef struct ListRep *List;
typedef List TrapList;
typedef int trapType; 


List newTrapList(); // create new empty lis
void dropTrapList(List); // free memory used by list
void showTrapList(List); // display as [1,2,3...]
void TrapListInsert(List,Item); // add item into list
void TrapListDelete(List,Key); // remove item
Item *TrapListSearch(List,Key); // return item with key
int  TrapListLength(List); // # items in list

#endif
