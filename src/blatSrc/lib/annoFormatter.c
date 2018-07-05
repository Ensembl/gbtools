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

/* annoFormatter -- aggregates, formats and writes output from multiple sources */

/* Copyright (C) 2013 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */

#include "annoFormatter.h"

struct annoOption *annoFormatterGetOptions(struct annoFormatter *self)
/* Return supported options and current settings.  Callers can modify and free when done. */
{
return annoOptionCloneList(self->options);
}

void annoFormatterSetOptions(struct annoFormatter *self, struct annoOption *newOptions)
/* Free old options and use clone of newOptions. */
{
annoOptionFreeList(&(self->options));
self->options = annoOptionCloneList(newOptions);
}

void annoFormatterFree(struct annoFormatter **pSelf)
/* Free self. This should be called at the end of subclass close methods, after
 * subclass-specific connections are closed and resources are freed. */
{
if (pSelf == NULL)
    return;
annoOptionFreeList(&((*pSelf)->options));
freez(pSelf);
}
