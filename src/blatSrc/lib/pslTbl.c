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

/* table of psl alignments, grouped by query */

/* Copyright (C) 2005 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */

#include "common.h"
#include "pslTbl.h"
#include "psl.h"
#include "hash.h"
#include "linefile.h"
#include "localmem.h"

static struct pslQuery *pslQueryGet(struct pslTbl *pslTbl,
                                    char *qName)
/* get pslQuery object for qName, creating if it doesn't exist. */
{
struct hashEl *qHel = hashStore(pslTbl->queryHash, qName);
if (qHel->val == NULL)
    {
    struct pslQuery *pslQuery;
    lmAllocVar(pslTbl->queryHash->lm, pslQuery);
    pslQuery->qName = qHel->name;
    qHel->val = pslQuery;
    }
return qHel->val;
}

static void loadPsl(struct pslTbl *pslTbl, char **row)
/* load a psl record into the table */
{
struct psl *psl = pslLoadLm(row, pslTbl->queryHash->lm);
struct pslQuery *pslQuery = pslQueryGet(pslTbl, psl->qName);
slAddHead(&pslQuery->psls, psl);
}

static void loadPsls(struct pslTbl *pslTbl, char *pslFile)
/* load a psl records into the table */
{
struct lineFile *lf = lineFileOpen(pslFile, TRUE);
char *row[PSL_NUM_COLS];
while (lineFileNextRowTab(lf, row, ArraySize(row)))
    loadPsl(pslTbl, row);
lineFileClose(&lf);
}

struct pslTbl *pslTblNew(char *pslFile, char *setName)
/* construct a new object, loading the psl file.  If setName is NULL, the file
* name is saved as the set name. */
{
struct pslTbl *pslTbl;
AllocVar(pslTbl);
pslTbl->setName = (setName == NULL) ? cloneString(pslFile)
    : cloneString(setName);
pslTbl->queryHash = hashNew(22);
loadPsls(pslTbl, pslFile);
return pslTbl;
}

void pslTblFree(struct pslTbl **pslTblPtr)
/* free object */
{
struct pslTbl *pslTbl = *pslTblPtr;
if (pslTbl != NULL)
    {
    /* pslQuery and psl objects are in local mem */
    freeMem(pslTbl->setName);
    hashFree(&pslTbl->queryHash);
    freeMem(pslTbl);
    }
}

void pslTblFreeList(struct pslTbl **pslTblList)
/* free list of pslTbls */
{
struct pslTbl *pslTbl = *pslTblList;
while (pslTbl != NULL)
    {
    struct pslTbl *pslTblDel = pslTbl;
    pslTbl = pslTbl->next;
    pslTblFree(&pslTblDel);
    }
*pslTblList = NULL;
}

/*
 * Local Variables:
 * c-file-style: "jkent-c"
 * End:
 */

