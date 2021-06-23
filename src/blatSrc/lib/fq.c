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

/* fq - stuff for doing i/o on fastq files. */

/* Copyright (C) 2014 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */

#include "common.h"
#include "linefile.h"
#include "fq.h"

struct fq *fqReadNext(struct lineFile *lf)
/* Read next record, return it as fq. */
{
struct fq *fq;
AllocVar(fq);
char *line;

/* Deal with initial line starting with '@' */
if (!lineFileNextReal(lf, &line))
    return FALSE;
if (line[0] != '@')
    {
    errAbort("Expecting line starting with '@' got %s line %d of %s", 
	line, lf->lineIx, lf->fileName);
    }
fq->header = cloneString(line);

/* Deal with line containing sequence. */
if (!lineFileNext(lf, &line,  NULL))
    errAbort("%s truncated in middle of record", lf->fileName);
fq->dna = cloneString(line);

/* Check for + line */
if (!lineFileNext(lf, &line,  NULL))
    errAbort("%s truncated in middle of record", lf->fileName);
if (line[0] != '+')
    errAbort("Expecting + line %d of %s", lf->lineIx, lf->fileName);

/* Get quality line */
if (!lineFileNext(lf, &line,  NULL))
    errAbort("%s truncated in middle of record", lf->fileName);
fq->quality = (unsigned char *)cloneString(line);
return fq;
}

void fqFree(struct fq **pFq)
/* Free up *pFq and set it to NULL */
{
struct fq *fq = *pFq;
if (fq != NULL)
    {
    freeMem(fq->header);
    freeMem(fq->dna);
    freeMem(fq->quality);
    freez(pFq);
    }
}

void fqWriteNext(struct fq *input, FILE *f)
/*  Writes a single fastq structure to the file provided. */
{
fprintf(f,"%s\n",input->header);
fprintf(f,"%s\n",input->dna);
fprintf(f,"%s\n","+");
fprintf(f,"%s\n",input->quality);
}
