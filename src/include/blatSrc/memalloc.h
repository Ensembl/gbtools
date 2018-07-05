/*
 * Copyright [2018] EMBL-European Bioinformatics Institute
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Let the user redirect where memory allocation/deallocation
 * happens.  'careful' routines help debug scrambled heaps. 
 *
 * This file is copyright 2002 Jim Kent, but license is hereby
 * granted for all use - public, private or commercial. */

#ifndef MEMALLOC_H
#define MEMALLOC_H

struct memHandler
    {
    struct memHandler *next;
    void * (*alloc)(size_t size);
    void (*free)(void *vpt);
    void * (*realloc)(void* vpt, size_t size);
    };

struct memHandler *pushMemHandler(struct memHandler *newHandler);
/* Use newHandler for memory requests until matching popMemHandler.
 * Returns previous top of memory handler stack. */

struct memHandler *popMemHandler();
/* Removes top element from memHandler stack and returns it. */

void setDefaultMemHandler();
/* Sets memHandler to the default. */

void pushCarefulMemHandler(size_t maxAlloc);
/* Push the careful (paranoid, conservative, checks everything)
 * memory handler  top of the memHandler stack and use it. */

void carefulCheckHeap();
/* Walk through allocated memory and make sure that all cookies are
 * in place. Only walks through what's been done since 
 * pushCarefulMemHandler(). */

int carefulCountBlocksAllocated();
/* How many memory items are allocated? (Since called
 * pushCarefulMemHandler(). */

size_t carefulTotalAllocated();
/* Return total bases allocated */

void setMaxAlloc(size_t s);
/* Set large allocation limit. */

void memTrackerStart();
/* Push memory handler that will track blocks allocated so that
 * they can be automatically released with memTrackerEnd().  */

void memTrackerEnd();
/* Free any remaining blocks and pop tracker memory handler. */

#endif /* MEMALLOC_H */

