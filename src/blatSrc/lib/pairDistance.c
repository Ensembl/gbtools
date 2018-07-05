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

/* pairDistance - help manage systems where you have a list of distances between pairs of
 * elements */

#include "common.h"
#include "linefile.h"
#include "hash.h"
#include "pairDistance.h"
#include "sqlNum.h"

struct pairDistance *pairDistanceReadAll(char *fileName)
/* Read in file of format <a> <b> <distance> into list of pairs */
{
struct lineFile *lf = lineFileOpen(fileName, TRUE);
struct pairDistance *list = NULL, *pair;
char *row[3];
while (lineFileRow(lf, row))
    {
    AllocVar(pair);
    pair->a = cloneString(row[0]);
    pair->b = cloneString(row[1]);
    pair->distance = sqlDouble(row[2]);
    slAddHead(&list, pair);
    }
slReverse(&list);
return list;
}

struct hash *pairDistanceHashList(struct pairDistance *pairList)
/* Return hash of all pairs keyed by pairDistanceName function on pair with pair values */
{
struct hash *hash = hashNew(0);
struct pairDistance *pair;
for (pair = pairList; pair != NULL; pair = pair->next)
    {
    char name[2*PATH_LEN];
    pairDistanceName(pair->a, pair->b, name, sizeof(name));
    hashAdd(hash, name, pair);
    pairDistanceName(pair->b, pair->a, name, sizeof(name));
    hashAdd(hash, name, pair);
    }
return hash;
}

double pairDistanceHashLookup(struct hash *pairHash, char *aName, char *bName)
/* Return distance between a and b. */
{
char name[2*PATH_LEN];
pairDistanceName(aName, bName, name, sizeof(name));
struct pairDistance *pair = hashMustFindVal(pairHash, name);
return pair->distance;
}

void pairDistanceInvert(struct pairDistance *list)
/* Go through and reverse distances, and make them positive. */
{
if (list == NULL)
    return;
double min = list->distance, max = list->distance;
struct pairDistance *pair;
for (pair = list; pair != NULL; pair = pair->next)
    {
    double distance = pair->distance;
    if (distance < min) min = distance;
    if (distance > max) max = distance;
    }
double range = max - min;
for (pair = list; pair != NULL; pair = pair->next)
    {
    double old = pair->distance;
    pair->distance = range - (old - min);
    }
}

char *pairDistanceName(char *a, char *b, char *outBuf, int outBufSize)
/* Return name for pair */
{
safef(outBuf, outBufSize, "%s\t%s", a, b);
return outBuf;
}

