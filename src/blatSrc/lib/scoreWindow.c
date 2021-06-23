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

/* scoreWindow - find window with most matches to a given char */

/* Copyright (C) 2011 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */

#include "common.h"


int scoreWindow(char c, char *s, int size, int *score, int *start, int *end, int match, int misMatch)
/* simple program to find max scoring window representing string of char c in a string s of size size */
/* index of max score is returned , match and misMatch are the scores to assign, suggested defaults are match=1 and misMatch=1*/
/* when used for scoring polyA tails, set c='A' for positive strand  or c='T' for neg strand */
/* start, end are returned pointing to the start and end of the highest scoring window in s */
{
int i=0, j=0, max=0, count = 0; 

*end = 0;

for (i=0 ; i<size ; i++)
    {
    int prevScore = (i > 0) ? score[i-1] : 0;

    if (toupper(s[i]) == toupper(c) )
        score[i] = prevScore+match;
    else
        score[i] = prevScore-misMatch;
    if (score[i] >= max)
        {
        max = score[i];
        *end = i;
        /* traceback to find start */
        for (j=i ; j>=0 ; j--)
            if (score[j] == 0)
                {
                *start = j+1;
                break;
                }
        }
    if (score[i] < 0) 
        score[i] = 0;
    }
assert (*end < size);

for (i=*start ; i<=*end ; i++)
    {
    assert (i < size);
    if (toupper(s[i]) == toupper(c) )
        count++;
    }
return count;
}
