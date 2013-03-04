//@date 2011-03-10
#include "typedef.h"
#include <pthread.h>

#ifndef LOCK_H
#define LOCK_H

DECLS_BEGIN

struct _Lock;
typedef struct _Lock Lock;

Lock*   lock_create(void);
void  lock_destroy(Lock* thiz);

Ret   lock_read(Lock* thiz);
Ret   lock_write(Lock* thiz);

Ret   lock_unread(Lock* thiz);
Ret   lock_unwrite(Lock* thiz);

DECLS_END

#endif
