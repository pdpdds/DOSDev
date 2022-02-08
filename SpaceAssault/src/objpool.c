#include "objpool.h"
#include <stdlib.h>

#ifndef OBJPOOL_SIZE
#define OBJPOOL_SIZE            200
#endif

static sObject g_objpool[OBJPOOL_SIZE];

void objpool_clear(void) {

    memset(g_objpool, 0, sizeof(g_objpool));
}

hsObject objpool_alloc_flags(hcsObjType type, char flags) {

    hsObject obj;

    for (obj = &g_objpool[0]; obj < &g_objpool[OBJPOOL_SIZE]; obj++) {
    
        if (obj->type != NULL)
            continue;
        
        obj->type           = type;
        obj->flags          = flags;
        obj->physical       = type->physical;
        obj->speed.x        = 0;
        obj->speed.y        = 0;
        obj->ttl            = 0;
        obj->damage_now     = 0;
        obj->damage_total   = 0;
        return obj;
    }
            
    return NULL;
}

void objpool_free(hsObject slot) {

    slot->type = NULL;
}

hsObject objpool_next(hsObject slot) {

    slot = (slot == NULL) ? &g_objpool[0] : (slot + 1);

    for (/*nothing*/; slot < &g_objpool[OBJPOOL_SIZE]; slot++)
        if (slot->type != NULL)
            return slot;
    
    return NULL;
}

hsObject objpool_next_without_flags(hsObject slot, char flags) {

    slot = (slot == NULL) ? &g_objpool[0] : (slot + 1);

    for (/*nothing*/; slot < &g_objpool[OBJPOOL_SIZE]; slot++)
        if (slot->type != NULL && !(slot->flags & flags))
            return slot;
    
    return NULL;
}

void objpool_garbage(void) {

    hsObject obj;

    for (obj = &g_objpool[0]; obj < &g_objpool[OBJPOOL_SIZE]; obj++)
        if (obj->flags & OBJ_FLG_DESTROY)
            obj->type = NULL;
}
