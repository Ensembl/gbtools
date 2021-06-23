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

/* annoStreamVcf -- subclass of annoStreamer for VCF files */

#ifndef ANNOSTREAMVCF_H
#define ANNOSTREAMVCF_H

#include "annoStreamer.h"

struct annoStreamer *annoStreamVcfNew(char *fileOrUrl, boolean isTabix, struct annoAssembly *aa,
				      int maxRecords);
/* Create an annoStreamer (subclass) object from a VCF file, which may
 * or may not have been compressed and indexed by tabix. */

#endif//ndef ANNOSTREAMVCF_H
