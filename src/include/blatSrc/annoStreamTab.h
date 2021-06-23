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

/* annoStreamTab -- subclass of annoStreamer for tab-separated text files/URLs */

#ifndef ANNOSTREAMTAB_H
#define ANNOSTREAMTAB_H

#include "annoStreamer.h"

struct annoStreamer *annoStreamTabNew(char *fileOrUrl, struct annoAssembly *aa,
				      struct asObject *asObj);
/* Create an annoStreamer (subclass) object from a tab-separated text file/URL
 * whose columns are described by asObj (possibly excepting bin column at beginning). */

#endif//ndef ANNOSTREAMTAB_H
