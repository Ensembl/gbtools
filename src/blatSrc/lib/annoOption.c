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

/* Copyright (C) 2013 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */

#include "annoOption.h"

struct annoOption *annoOptionCloneList(struct annoOption *list)
/* Return a newly allocated copy of list. */
{
struct annoOption *newList = NULL, *afo;
for (afo = list;  afo != NULL;  afo = afo->next)
    {
    struct annoOption *newAfo = CloneVar(afo);
    newAfo->spec.name = cloneString(afo->spec.name);
    newAfo->value = NULL;
    unsigned opFlags = afo->spec.flags;
    if (opFlags & OPTION_MULTI)
	{
	switch (opFlags & OPTION_TYPE_MASK)
	    {
	    case OPTION_STRING:
		newAfo->value = slNameCloneList((struct slName *)(afo->value));
		break;
	    default:
		errAbort("annoOptionCloneList: OPTION_MULTI implemented only for "
			 "OPTION_STRING (not 0x%x)", opFlags);
	    }
	}
    else
	{
	switch (opFlags & OPTION_TYPE_MASK)
	    {
	    // For numeric singleton values, we are overloading value.
	    case OPTION_DOUBLE:
	    case OPTION_FLOAT:
	    case OPTION_LONG_LONG:
	    case OPTION_INT:
	    case OPTION_BOOLEAN:
		newAfo->value = afo->value;
		break;
	    case OPTION_STRING:
		newAfo->value = cloneString((char *)afo->value);
		break;
	    default:
		errAbort("annoOptionCloneList: unrecognized op type 0x%x", opFlags);
	    }
	}
    slAddHead(&newList, newAfo);
    }
slReverse(&newList);
return newList;
}

void annoOptionFreeList(struct annoOption **pList)
/* Free the same things that we clone above. */
{
if (pList == NULL)
    return;
struct annoOption *afo, *nextAfo;
for (afo = *pList;  afo != NULL;  afo = nextAfo)
    {
    nextAfo = afo->next;
    freeMem(afo->spec.name);
    unsigned opFlags = afo->spec.flags;
    if (opFlags & OPTION_MULTI)
	{
	switch (opFlags & OPTION_TYPE_MASK)
	    {
	    case OPTION_STRING:
		slNameFreeList(&(afo->value));
		break;
	    default:
		errAbort("annoOptionFreeList: OPTION_MULTI implemented only for "
			 "OPTION_STRING (not 0x%x)", opFlags);
	    }
	}
    else
	{
	switch (opFlags & OPTION_TYPE_MASK)
	    {
	    // No need to free overloaded numeric values
	    case OPTION_DOUBLE:
	    case OPTION_FLOAT:
	    case OPTION_LONG_LONG:
	    case OPTION_INT:
	    case OPTION_BOOLEAN:
		break;
	    case OPTION_STRING:
		freeMem(afo->value);
		break;
	    default:
		errAbort("annoOptionFreeList: unrecognized op type 0x%x", opFlags);
	    }
	}
    freeMem(afo);
    }
*pList = NULL;
}

