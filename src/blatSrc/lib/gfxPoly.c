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

/* gfxPoly - two dimensional polygon. */

#include "common.h"
#include "gfxPoly.h"


struct gfxPoly *gfxPolyNew()
/* Create new (empty) polygon */
{
struct gfxPoly *poly;
AllocVar(poly);
return poly;
}

void gfxPolyFree(struct gfxPoly **pPoly)
/* Free up resources associated with polygon */
{
struct gfxPoly *poly = *pPoly;
if (poly != NULL)
    {
    if (poly->lastPoint != NULL)
	{
	poly->lastPoint->next = NULL;
	slFreeList(&poly->ptList);
	}
    freez(pPoly);
    }
}

void gfxPolyAddPoint(struct gfxPoly *poly, int x, int y)
/* Add point to polygon. */
{
struct gfxPoint *pt;
poly->ptCount += 1;
AllocVar(pt);
pt->x = x;
pt->y = y;
if (poly->ptList == NULL)
    {
    poly->ptList = poly->lastPoint = pt;
    pt->next = pt;
    }
else
    {
    poly->lastPoint->next = pt;
    pt->next = poly->ptList;
    poly->lastPoint = pt;
    }
}

