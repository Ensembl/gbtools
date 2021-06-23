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

/* hmmpfamParse - Parse hmmpfam files.. */

#ifndef HMMPFAMPARSE_H
#define HMMPFAMPARSE_H

struct hpfDomain
/* Describes amino acids covered by one use of model. */
    {
    struct hpfDomain *next;
    int qStart, qEnd;	/* Where this is in query sequence. */
    int hmmStart, hmmEnd; /* Where this is in profile HMM. */
    double score;	/* Some sort of bit score. */
    double eVal;	/* Expectation value. */
    };

struct hpfModel 
/* Results for a single model for a single query */
    {
    struct hpfModel *next;
    char *name;	/* Model name */
    char *description; /* Longer description */
    double score;	/* Some sort of bit score. */
    double eVal;	/* Expectation value. */
    struct hpfDomain *domainList;	/* All places model occurs in query */
    };

void hpfModelFree(struct hpfModel **pMod);
/* Free memory associated with hpfModel */

void hpfModelFreeList (struct hpfModel **pList);
/* Free memory associated with list of hpfModels.  */

struct hpfResult
/* Result for a single query sequence. */
    {
    struct hpfResult *next;
    char *name;	/* Query sequence name. */
    struct hpfModel *modelList;  /* All models that hit query */
    };

void hpfResultFree(struct hpfResult **pHr);
/* Free memory associated with hpfResult */

void hpfResultFreeList(struct hpfResult **pList);
/* Free memory associated with list of hpfResults. */


struct hpfModel *hpfFindResultInModel(struct hpfResult *hr, char *modName);
/* Look for named result in model. */

struct hpfResult *hpfNext(struct lineFile *lf);
/* Parse out next record in hmmpfam result file. */


#endif /* HMMPFAMPARSE_H */
