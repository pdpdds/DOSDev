#ifndef __OBJPOOL_H
#define __OBJPOOL_H

#include "shooter.h"

void objpool_clear(void);
void objpool_garbage(void);

hsObject objpool_alloc_flags(hcsObjType type, char flags);
#define objpool_alloc(type)             objpool_alloc_flags((type), 0)
#define objpool_alloc_inactive(type)    objpool_alloc_flags((type), OBJ_FLG_INACTIVE)

void objpool_free(hsObject slot);

hsObject objpool_next(hsObject slot);
hsObject objpool_next_without_flags(hsObject slot, char flags);
#define objpool_next_active(slot)       objpool_next_without_flags((slot), OBJ_FLG_INACTIVE)

#endif
