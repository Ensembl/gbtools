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

/* pslTransMap - transitive mapping of an alignment another sequence, via a
 * common alignment */
#ifndef PSLTRANSMAP_H
#define PSLTRANSMAP_H

enum pslTransMapOpts
/* option set for pslTransMap */
{
    pslTransMapNoOpts     = 0x00,  /* no options */
    pslTransMapKeepTrans  = 0x01   /* keep translated alignment strand */
};

struct psl* pslTransMap(unsigned opts, struct psl *inPsl, struct psl *mapPsl);
/* map a psl via a mapping psl, a single psl is returned, or NULL if it
 * couldn't be mapped. */

#endif
