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

/* fq - stuff for doing i/o on fastq files. */

#ifndef FQ_H
#define FQ_H

struct fq
/* Representation of record as 3 lines of text, not further parsed.  We omit the 1 of 4 lines in the
 * fastq record, using '+' as a marker between dna and quality. */
    {
    struct fq *next;
    char *header;   // This line starts with @ and contains the sequence name plus other stuff
    char *dna;  // ACGT and on occassional N normally
    unsigned char *quality; // Might be 33-based (Sanger) or 64 based (Solexa)
    };

struct fq *fqReadNext(struct lineFile *lf);
/* Read next record, return a fq struct. */

void fqFree(struct fq **pFq);
/* Free up *pFq and set it to NULL */

void fqWriteNext(struct fq *input, FILE *f);
/*  Writes a single fastq structure to the file provided. */

#endif /* FQ_H */

