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

#ifndef PAIRDISTANCE_H
#define PAIRDISTANCE_H

struct pairDistance
/* A pair of items and the distance between them. */
    {
    struct pairDistance *next;
    char *a;	/* First in pair */
    char *b;	/* Second in pair */
    double distance;  /* Distance between two */
    };

struct pairDistance *pairDistanceReadAll(char *fileName);
/* Read in file of format <a> <b> <distance> into list of pairs */

struct hash *pairDistanceHashList(struct pairDistance *pairList);
/* Return hash of all pairs keyed by pairName function on pair with pair values */

double pairDistanceHashLookup(struct hash *pairHash, char *aName, char *bName);
/* Return distance between a and b. */

void pairDistanceInvert(struct pairDistance *list);
/* Go through and reverse distances, and make them positive. */

char *pairDistanceName(char *a, char *b, char *outBuf, int outBufSize);
/* Return name for pair */

#endif /* PAIRDISTANCE_H */
