#ifndef ITEM_H
#define ITEM_H

#include "Places.h"
#include "Game.h"


typedef int Item;
typedef int Key;
typedef Item Trap;

#define eq(a,b) ItemEQ(a,b)
#define gt(a,b) ItemGT(a,b)
#define key(a)  ItemKey(a)

#define ItemCopy(i)     (i)
#define ItemKey(i)      (i)
#define ItemEQ(i1,i2)   ((i1) == (i2))
#define ItemLT(i1,i2)   ((i1) < (i2))
#define ItemGT(i1,i2)   ((i1) > (i2))
#define ItemShow(i)     printf("%d",(i))
#define ItemDrop(i)     ;

typedef struct traps{
	Place place;
	Round round;
	int trap_type;
} Traps;


#endif

