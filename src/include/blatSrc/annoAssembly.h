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

/* annoAssembly -- basic metadata about an assembly for the annoGrator framework. */

#ifndef ANNOASSEMBLY_H
#define ANNOASSEMBLY_H

#include "common.h"
#include "hash.h"

struct annoAssembly
/* Basic information about a genome assembly. */
    {
    char *name;			// UCSC symbolic name for assembly, e.g. "hg19"
    struct twoBitFile *tbf;	// Opened twoBit sequence file for assembly
    char *twoBitPath;		// twoBit file name
    struct hash *seqSizes;	// cache of sequence names to sizes (twoBitSeqSize does a seek&read)
    struct dnaSeq *curSeq;	// cache of most recently accessed sequence (chrom/scaffold)
    };

struct annoAssembly *annoAssemblyNew(char *name, char *twoBitPath);
/* Return an annoAssembly with open twoBitFile. */

struct slName *annoAssemblySeqNames(struct annoAssembly *aa);
/* Return a list of sequence names in this assembly. */

uint annoAssemblySeqSize(struct annoAssembly *aa, char *seqName);
/* Return the number of bases in seq which must be in aa's twoBitFile. */

void annoAssemblyGetSeq(struct annoAssembly *aa, char *seqName, uint start, uint end,
			char *buf, size_t bufSize);
/* Copy sequence to buf; bufSize must be at least end-start+1 chars in length. */

void annoAssemblyClose(struct annoAssembly **pAa);
/* Close aa's twoBitFile and free mem. */

#endif//ndef ANNOASSEMBLY_H
