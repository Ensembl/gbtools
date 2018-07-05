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

/* gfClientLib - stuff that both blat and pcr clients of
 * genoFind use. */

#ifndef GFCLIENTLIB_H
#define GFCLIENTLIB_H

void gfClientFileArray(char *fileName, char ***retFiles, int *retFileCount);
/* Check if file if .2bit or .nib or .fa.  If so return just that
 * file in a list of one.  Otherwise read all file and treat file
 * as a list of filenames.  */

bioSeq *gfClientSeqList(int fileCount, char *files[], 
	boolean isProt, boolean isTransDna, char *maskType, 
	float minRepDivergence, boolean showStatus);
/* From an array of .fa and .nib file names, create a
 * list of dnaSeqs, which are set up so that upper case is masked and lower case
 * is unmasked sequence. (This is the opposite of the input, but set so that
 * we can use lower case as our primary DNA sequence, which historically predates
 * our use of lower case masking.)  Protein sequence on the other hand is
 * all upper case. */

void gfClientUnmask(struct dnaSeq *seqList);
/* Unmask all sequences. */

#endif /* GFCLIENTLIB_H */

