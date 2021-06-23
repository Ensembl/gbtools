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

/* aliType - some definitions for type of alignment. */

#ifndef ALITYPE_H
#define ALITYPE_H

enum gfType
/* Types of sequence genoFind deals with. */
    {
    gftDna = 0,		/* DNA (genomic) */
    gftRna = 1,		/* RNA */
    gftProt = 2,         /* Protein. */
    gftDnaX = 3,		/* Genomic DNA translated to protein */
    gftRnaX = 4,         /* RNA translated to protein */
    };

char *gfTypeName(enum gfType type);
/* Return string representing type. */

enum gfType gfTypeFromName(char *name);
/* Return type from string. */

enum ffStringency
/* How tight of a match is required. */
    {
    ffExact = 0,	/* Only an exact match will do. */

    ffCdna = 1,		/* Near exact.  Tolerate long gaps in target (genomic) */
    ffTight = 2,        /* Near exact.  Not so tolerant of long gaps in target. */
    ffLoose = 3,        /* Less exact. */
    };

#endif /* ALITYPE_H */
