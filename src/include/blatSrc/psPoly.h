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

/* psPoly - two dimensional polygon. */

#ifndef PSPOLY_H
#define PSPOLY_H

struct psPoint
/* A two-dimensional point, typically in pixel coordinates. */
    {
    struct psPoint *next;
    double x, y;		/* Position */
    };

struct psPoly
/* A two-dimensional polygon */
    {
    struct psPoly *next;
    int ptCount;		/* Number of points. */
    struct psPoint *ptList;	/* First point in list, which is circular. */
    struct psPoint *lastPoint;	/* Last point in list. */
    };

struct psPoly *psPolyNew();
/* Create new (empty) polygon */

void psPolyFree(struct psPoly **pPoly);
/* Free up resources associated with polygon */

void psPolyAddPoint(struct psPoly *poly, double x, double y);
/* Add point to polygon. */

#endif /* PSPOLY_H */
