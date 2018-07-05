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

/* boxClump - put together 2 dimensional boxes that
 * overlap with each other into clumps. */

#ifndef BOXCLUMP_H
#define BOXCLUMP_H

struct boxIn
/* Input to box clumper. */
    {
    struct boxIn *next;		 /* Next in list. */
    void *data;			 /* Some user-associated data. */
    int qStart, qEnd;		 /* Range covered in query. */
    int tStart, tEnd;		 /* Range covered in target. */
    };

struct boxClump
/* Output of box clumper. */
    {
    struct boxClump *next;	 /* Next in list. */
    struct boxIn *boxList;	 /* List of boxes in this clump. */
    int boxCount;		 /* Count of boxes in this clump. */
    int qStart, qEnd;		 /* Expanse of clump in query. */
    int tStart, tEnd;		 /* Expanse of clump in target. */
    };

void boxClumpFree(struct boxClump **pClump);
/* Free boxClump. */

void boxClumpFreeList(struct boxClump **pList);
/* Free list of boxClumps. */

int boxClumpCmpCount(const void *va, const void *vb);
/* Compare to sort based on count of boxes. */

struct boxClump *boxFindClumps(struct boxIn **pBoxList);
/* Convert list of boxes to a list of clumps.  Clumps
 * are collections of boxes that overlap.  Note that
 * the original boxList is overwritten as the boxes
 * are moved from it to the clumps. */

#endif /* BOXCLUMP_H */
