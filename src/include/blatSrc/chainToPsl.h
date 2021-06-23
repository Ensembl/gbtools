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

/* chainToPsl - convert between chains and psl.  Both of these
 * are alignment formats that can handle gaps in both strands
 * and do not include the sequence itself. */

#ifndef CHAINTOPSL_H
#define CHAINTOPSL_H

#ifndef PSL_H
#include "psl.h"
#endif

#ifndef CHAINBLOCK_H
#include "chainBlock.h"
#endif

struct psl *chainToPsl(struct chain *chain);
/* chainToPsl - convert chain to psl.  This does not fill in
 * the match, repMatch, mismatch, and N fields since it needs
 * the sequence for that.  It does fill in the rest though. */

struct psl *chainToFullPsl(struct chain *chain, 
	struct dnaSeq *query,   /* Forward query sequence. */
	struct dnaSeq *rQuery,	/* Reverse complemented query sequence. */
	struct dnaSeq *target);
/* Convert chainList to pslList, filling in matches, N's etc. */

#endif /* CHAINTOPSL_H */

