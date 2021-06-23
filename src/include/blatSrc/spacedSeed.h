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

/* spacedSeed - stuff to help with spaced seeds for alignments. */

#ifndef SPACEDSEED_H
#define SPACEDSEED_H

extern char *spacedSeeds[];
/* Array of spaced seeds in format with '1' for cares, '0' for don't care. */

int spacedSeedMaxWeight();
/* Return max weight of spaced seed. */

int *spacedSeedOffsets(int weight);
/* Return array with offsets for seed of given weight. */

int spacedSeedSpan(int weight);
/* Return span of seed of given weight */

#endif /*SPACEDSEED_H */

