/*
 * Copyright [2018-2021] EMBL-European Bioinformatics Institute
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

/* colHash - stuff for fast lookup of index given an
 * rgb value. */

/* Copyright (C) 2011 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */

#include "common.h"
#include "memgfx.h"
#include "colHash.h"


struct colHash *colHashNew()
/* Get a new color hash. */
{
struct colHash *cHash;
AllocVar(cHash);
cHash->freeEl = cHash->elBuf;
return cHash;
}

void colHashFree(struct colHash **pEl)
/* Free up color hash. */
{
freez(pEl);
}

struct colHashEl *colHashAdd(struct colHash *cHash, 
	unsigned r, unsigned g, unsigned b, int ix)
/* Add new element to color hash. */
{
struct colHashEl *che = cHash->freeEl++, **pCel;
che->col.r = r;
che->col.g = g;
che->col.b = b;
che->ix = ix;
pCel = &cHash->lists[colHashFunc(r,g,b)];
slAddHead(pCel, che);
return che;
}

struct colHashEl *colHashLookup(struct colHash *cHash, 
	unsigned r, unsigned g, unsigned b)
/* Lookup value in hash. */
{
struct colHashEl *che;
for (che = cHash->lists[colHashFunc(r,g,b)]; che != NULL; che = che->next)
    if (che->col.r == r && che->col.g == g && che->col.b == b)
	return che;
return NULL;
}

