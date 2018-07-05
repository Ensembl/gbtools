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
#ifndef PSLTBL_H
#define PSLTBL_H

struct psl;
struct hash;

struct pslQuery
/* object containing PSLs for a single query */
{
    struct pslQuery *next;
    char *qName;          /* qName, memory not owned here */
    struct psl *psls;     /* alignments */
};

struct pslTbl
/* table of psl alignments */
{
    struct pslTbl *next;               /* next psl table in a list */
    char *setName;                     /* name identifying the set of psl.
                                        * maybe file name, or  other name */
    struct hash *queryHash;            /* hash of pslQuery objects */
};

struct pslTbl *pslTblNew(char *pslFile, char *setName);
/* construct a new object, loading the psl file.  If setName is NULL, the file
* name is saved as the set name. */

void pslTblFree(struct pslTbl **pslTblPtr);
/* free object */

void pslTblFreeList(struct pslTbl **pslTblList);
/* free list of pslTbls */

#endif

/*
 * Local Variables:
 * c-file-style: "jkent-c"
 * End:
 */

