/*
 * Copyright (c) 2007-2011 by Hewlett-Packard Company. All rights reserved.
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#ifndef GC_DISCLAIM_H
#define GC_DISCLAIM_H

#include "gc.h"

/* This API is defined only if the library has been suitably compiled   */
/* (i.e. with ENABLE_DISCLAIM defined).                                 */

/* Prepare the object kind used by GC_finalized_malloc.  Call it from   */
/* your initialization code or, at least, at some point before using    */
/* finalized allocations.  The function is thread-safe.                 */
GC_API void GC_CALL GC_init_finalized_malloc(void);

/* Type of a disclaim call-back, always stored along with closure data  */
/* passed as the second argument.                                       */
typedef int (GC_CALLBACK * GC_disclaim_proc)(void * /*obj*/, void * /*cd*/);

/* Register "proc" to be called on each object of "kind" ready to be    */
/* reclaimed.  If "proc" returns non-zero, the collector will not       */
/* reclaim the object on this GC cycle.  Objects reachable from "proc"  */
/* will be protected from collection if "mark_from_all" is non-zero,    */
/* but at the expense that long chains of objects will take many cycles */
/* to reclaim.                                                          */
GC_API void GC_CALL GC_register_disclaim_proc(int /*kind*/,
                                              GC_disclaim_proc /*proc*/,
                                              void * /*cd*/,
                                              int /*mark_from_all*/);

/* The finalizer closure used by GC_finalized_malloc.                   */
struct GC_finalizer_closure {
    GC_finalization_proc proc;
    void *cd;
};

/* Allocate "size" bytes which is finalized by "fc".  This uses a       */
/* dedicated object kind with a disclaim procedure, and is more         */
/* efficient than GC_register_finalizer and friends.                    */
/* GC_init_finalized_malloc must be called before using this.           */
GC_API void *GC_CALL GC_finalized_malloc(size_t /*size*/,
                                        struct GC_finalizer_closure * /*fc*/);

#endif
