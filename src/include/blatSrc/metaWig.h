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

/* Interface class so that wigs and bigWigs look similar */

#ifndef METAWIG_H
#define METAWIG_H

#ifndef BBIFILE_H
#include "bbiFile.h"
#endif

enum metaWigType 
    {
    mwtSections,
    mwtBigWig,
    };

struct metaWig
/* Interface class so that wigs and bigWigs look similar */
    {
    struct metaWig *next;
    enum metaWigType type;

    /* For type mwtSections */
    struct bwgSection *sectionList;	/* List of all sections. */
    struct hash *chromHash; 		/* Hash value is first section in that chrom */
    struct lm *lm;			/* Where sectionList is allocated. */

    /* For type mwtBigWig */
    struct bbiFile *bwf;
    };

struct metaWig *metaWigOpen(char *fileName);
/* Wrap self around file.  Read all of it if it's wig, just header if bigWig. */

void metaWigClose(struct metaWig **pMw);
/* Close up metaWig file */

struct slName *metaWigChromList(struct metaWig *mw);
/* Return list of chromosomes covered in wig. */

struct bbiInterval *metaIntervalsForChrom(struct metaWig *mw, char *chrom, struct lm *lm);
/* Get sorted list of all intervals with data on chromosome. */

#endif /* METAWIG_H */

