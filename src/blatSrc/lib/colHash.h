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

/* Copyright (C) 2002 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */
#ifndef COLHASH_H
#define COLHASH_H

#define colHashFunc(r,g,b) (r+g+g+b)

struct colHashEl
/* An element in a color hash. */
    {
    struct colHashEl *next;	/* Next in list. */
    struct rgbColor col;	/* Color RGB. */
    int ix;			/* Color Index. */
    };

struct colHash
/* A hash on RGB colors. */
    {
    struct colHashEl *lists[4*256];	/* Hash chains. */
    struct colHashEl elBuf[256];	/* Buffer of elements. */
    struct colHashEl *freeEl;		/* Pointer to next free element. */
    };

struct colHash *colHashNew();
/* Get a new color hash. */

void colHashFree(struct colHash **pEl);
/* Free up color hash. */

struct colHashEl *colHashAdd(struct colHash *cHash, 
	unsigned r, unsigned g, unsigned b, int ix);
/* Add new element to color hash. */

struct colHashEl *colHashLookup(struct colHash *cHash, 
	unsigned r, unsigned g, unsigned b);
/* Lookup value in hash. */

#endif /* COLHASH_H */
