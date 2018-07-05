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

/* boxLump - This will lump together boxes that overlap into the smallest
 * box that encompasses the overlap.  It will put other boxes that 
 * fit in the encompassing box in there too. 
 *   It works by projecting the box list along one dimension at a
 * time looking for gaps between boxes. This is similar in function
 * to boxFindClumps, but a bit less precise, and quite a bit faster.
 * in some important cases. */

#ifndef BOXLUMP_H
#define BOXLUMP_H

#ifndef BOXCLUMP_H
#include "boxClump.h"
#endif

struct boxClump *boxLump(struct boxIn **pBoxList);
/* Convert list of boxes to a list of lumps.  The lumps
 * are a smaller number of boxes that between them contain
 * all of the input boxes.  Note that
 * the original boxList is overwritten as the boxes
 * are moved from it to the lumps. */

#endif /* BOXLUMP_H */


