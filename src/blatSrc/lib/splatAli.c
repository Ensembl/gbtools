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

/* splatAli.c was originally generated by the autoSql program, which also 
 * generated splatAli.h and splatAli.sql.  This module links the database and
 * the RAM representation of objects. */
/* This file is copyright 2008 Jim Kent, but license is hereby
 * granted for all use - public, private or commercial. */

#include "common.h"
#include "linefile.h"
#include "dystring.h"
#include "sqlNum.h"
#include "sqlList.h"
#include "splatAli.h"


void splatAliStaticLoad(char **row, struct splatAli *ret)
/* Load a row from splatAli table into ret.  The contents of ret will
 * be replaced at the next call to this function. */
{

ret->chrom = row[0];
ret->chromStart = sqlSigned(row[1]);
ret->chromEnd = sqlSigned(row[2]);
ret->alignedBases = row[3];
ret->score = sqlSigned(row[4]);
safecpy(ret->strand, sizeof(ret->strand), row[5]);
ret->readName = row[6];
}

struct splatAli *splatAliLoad(char **row)
/* Load a splatAli from row fetched with select * from splatAli
 * from database.  Dispose of this with splatAliFree(). */
{
struct splatAli *ret;

AllocVar(ret);
ret->chrom = cloneString(row[0]);
ret->chromStart = sqlSigned(row[1]);
ret->chromEnd = sqlSigned(row[2]);
ret->alignedBases = cloneString(row[3]);
ret->score = sqlSigned(row[4]);
safecpy(ret->strand, sizeof(ret->strand), row[5]);
ret->readName = cloneString(row[6]);
return ret;
}

struct splatAli *splatAliLoadAll(char *fileName) 
/* Load all splatAli from a whitespace-separated file.
 * Dispose of this with splatAliFreeList(). */
{
struct splatAli *list = NULL, *el;
struct lineFile *lf = lineFileOpen(fileName, TRUE);
char *row[7];

while (lineFileRow(lf, row))
    {
    el = splatAliLoad(row);
    slAddHead(&list, el);
    }
lineFileClose(&lf);
slReverse(&list);
return list;
}

struct splatAli *splatAliLoadAllByChar(char *fileName, char chopper) 
/* Load all splatAli from a chopper separated file.
 * Dispose of this with splatAliFreeList(). */
{
struct splatAli *list = NULL, *el;
struct lineFile *lf = lineFileOpen(fileName, TRUE);
char *row[7];

while (lineFileNextCharRow(lf, chopper, row, ArraySize(row)))
    {
    el = splatAliLoad(row);
    slAddHead(&list, el);
    }
lineFileClose(&lf);
slReverse(&list);
return list;
}

struct splatAli *splatAliCommaIn(char **pS, struct splatAli *ret)
/* Create a splatAli out of a comma separated string. 
 * This will fill in ret if non-null, otherwise will
 * return a new splatAli */
{
char *s = *pS;

if (ret == NULL)
    AllocVar(ret);
ret->chrom = sqlStringComma(&s);
ret->chromStart = sqlSignedComma(&s);
ret->chromEnd = sqlSignedComma(&s);
ret->alignedBases = sqlStringComma(&s);
ret->score = sqlSignedComma(&s);
sqlFixedStringComma(&s, ret->strand, sizeof(ret->strand));
ret->readName = sqlStringComma(&s);
*pS = s;
return ret;
}

void splatAliFree(struct splatAli **pEl)
/* Free a single dynamically allocated splatAli such as created
 * with splatAliLoad(). */
{
struct splatAli *el;

if ((el = *pEl) == NULL) return;
freeMem(el->chrom);
freeMem(el->alignedBases);
freeMem(el->readName);
freez(pEl);
}

void splatAliFreeList(struct splatAli **pList)
/* Free a list of dynamically allocated splatAli's */
{
struct splatAli *el, *next;

for (el = *pList; el != NULL; el = next)
    {
    next = el->next;
    splatAliFree(&el);
    }
*pList = NULL;
}

void splatAliOutput(struct splatAli *el, FILE *f, char sep, char lastSep) 
/* Print out splatAli.  Separate fields with sep. Follow last field with lastSep. */
{
if (sep == ',') fputc('"',f);
fprintf(f, "%s", el->chrom);
if (sep == ',') fputc('"',f);
fputc(sep,f);
fprintf(f, "%d", el->chromStart);
fputc(sep,f);
fprintf(f, "%d", el->chromEnd);
fputc(sep,f);
if (sep == ',') fputc('"',f);
fprintf(f, "%s", el->alignedBases);
if (sep == ',') fputc('"',f);
fputc(sep,f);
fprintf(f, "%d", el->score);
fputc(sep,f);
if (sep == ',') fputc('"',f);
fprintf(f, "%s", el->strand);
if (sep == ',') fputc('"',f);
fputc(sep,f);
if (sep == ',') fputc('"',f);
fprintf(f, "%s", el->readName);
if (sep == ',') fputc('"',f);
fputc(lastSep,f);
}

/* -------------------------------- End autoSql Generated Code -------------------------------- */

int splatAliCmpReadName(const void *va, const void *vb)
/* Compare two based on readName. Also separate secondarily on chrom position. */
{
const struct splatAli *a = *((struct splatAli **)va);
const struct splatAli *b = *((struct splatAli **)vb);
int diff = strcmp(a->readName, b->readName);
if (diff == 0)
    diff = a->chromStart - b->chromStart;
if (diff == 0)
    diff = a->chromEnd - b->chromEnd;
if (diff == 0)
    diff = a->strand - b->strand;
if (diff == 0)
    diff = strcmp(a->chrom, b->chrom);
return diff;
}

int splatAliScore(char *ali)
/* Score splat-encoded alignment. */
{
int score = 0;
char c;
while ((c = *ali++))
    {
    switch (c)
        {
	case 'a':
	case 'c':
	case 'g':
	case 't':
	    score -= 2;
	    break;
	case 'A':
	case 'C':
	case 'G':
	case 'T':
	    score += 2;
	    break;
	case 'n':
	case 'N':
	    break;
	case '^':
	    score -= 3;
	    ali += 1;
	    break;
	case '-':
	    score -= 3;
	    break;
	}
    }
return score;
}

void splatAliLookForBest(struct splatAli *start, struct splatAli *end, 
	int *retBestScore, int *retBestCount)
/* Scan through list from start up to but not including end (which may be NULL)
 * and figure out best score and number of elements in list with that score. */
{
int bestScore = 0, bestCount = 0;
struct splatAli *el;
for (el = start; el != end; el = el->next)
    {
    int score = splatAliScore(el->alignedBases);
    if (score >= bestScore)
        {
	if (score > bestScore)
	    {
	    bestScore = score;
	    bestCount = 1;
	    }
	else
	    bestCount += 1;
	}
    }
*retBestScore = bestScore;
*retBestCount = bestCount;
}

