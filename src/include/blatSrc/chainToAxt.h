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

/* chainToAxt - convert from chain to axt format. */

#ifndef CHAINTOAXT_H
#define CHAINTOAXT_H

struct axt *chainToAxt(struct chain *chain, 
	struct dnaSeq *qSeq, int qOffset,
	struct dnaSeq *tSeq, int tOffset, int maxGap, int maxChain);
/* Convert a chain to a list of axt's.  This will break
 * where there is a double-sided gap in chain, or 
 * where there is a single-sided gap greater than maxGap, or 
 * where there is a chain longer than maxChain.
 */

#endif /* CHAINTOAXT_H */
