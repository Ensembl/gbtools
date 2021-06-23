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

/* nibTwo - Something to let you transparently access either
 * .2bit or .nib files. */

#ifndef NIBTWO_H
#define NIBTWO_H

#ifndef NIB_H
#include "nib.h"
#endif

#ifndef TWOBIT_H
#include "twoBit.h"
#endif

struct nibTwoCache
/* This is a cache for either a directory full of nib files or a .2bit file. */
    {
    struct nibTwoCache *next;	/* Next in list */
    char *pathName;		/* Nib dir name or .2bit file name. */
    boolean isTwoBit;		/* True if this is a .2bit file. */
    struct twoBitFile *tbf;	/* Two bit file handle if any. */
    struct hash *nibHash;	/* Hash of nibInfo's if any. */
    };

struct nibTwoCache *nibTwoCacheNew(char *pathName);
/* Get something that will more or less transparently get sequence from 
 * nib files or .2bit. */ 

void nibTwoCacheFree(struct nibTwoCache **pNtc);
/* Free up resources associated with nibTwoCache. */

struct dnaSeq *nibTwoCacheSeq(struct nibTwoCache *ntc, char *seqName);
/* Return all of sequence. This will have repeats in lower case. */

struct dnaSeq *nibTwoCacheSeqPartExt(struct nibTwoCache *ntc, char *seqName, int start, int size,
                                     boolean doMask, int *retFullSeqSize);
/* Return part of sequence. If *retFullSeqSize is non-null then return full
 * size of sequence (not just loaded part) there.   Sequence will be lower
 * case if doMask is false, mixed case (repeats in lower)
 * if doMask is true. */

struct dnaSeq *nibTwoCacheSeqPart(struct nibTwoCache *ntc, char *seqName, 
	int start, int size, int *retFullSeqSize);
/* Return part of sequence. If *retFullSeqSize is non-null then return full size of
 * sequence (not just loaded part) there. This will have repeats in lower case. */

struct dnaSeq *nibTwoLoadOne(char *pathName, char *seqName);
/* Return sequence from a directory full of nibs or a .2bit file. 
 * The sequence will have repeats in lower case. */

int nibTwoGetSize(struct nibTwoCache *ntc, char *seqName);
/* Return size of sequence. */

#endif /* NIBTWO_H */

