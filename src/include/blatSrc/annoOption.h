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

/* annoOption -- optionSpec-style param plus its value */

#ifndef ANNOOPTION_H
#define ANNOOPTION_H

#include "options.h"

struct annoOption
/* A named and typed option and its value. */
    {
    struct annoOption *next;
    struct optionSpec spec;
    void *value;
    };

struct annoOption *annoOptionCloneList(struct annoOption *list);
/* Return a newly allocated copy of list. */

void annoOptionFreeList(struct annoOption **pList);
/* Free the same things that we clone above. */

#endif//ndef ANNOOPTION_H
