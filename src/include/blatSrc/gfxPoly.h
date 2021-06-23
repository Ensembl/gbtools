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

#ifndef GFXPOLY_H
#define GFXPOLY_H

struct gfxPoint
/* A two-dimensional point, typically in pixel coordinates. */
    {
    struct gfxPoint *next;
    int x, y;		/* Position */
    };

struct gfxPoly
/* A two-dimensional polygon */
    {
    struct gfxPoly *next;
    int ptCount;		/* Number of points. */
    struct gfxPoint *ptList;	/* First point in list, which is circular. */
    struct gfxPoint *lastPoint;	/* Last point in list. */
    };

struct gfxPoly *gfxPolyNew();
/* Create new (empty) polygon */

void gfxPolyFree(struct gfxPoly **pPoly);
/* Free up resources associated with polygon */

void gfxPolyAddPoint(struct gfxPoly *poly, int x, int y);
/* Add point to polygon. */

#endif /* GFXPOLY_H */
