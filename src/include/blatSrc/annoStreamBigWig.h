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

/* annoStreamBigWig -- subclass of annoStreamer for bigWig file or URL */

#ifndef ANNOSTREAMBIGWIG_H
#define ANNOSTREAMBIGWIG_H

#include "annoStreamer.h"

struct annoStreamer *annoStreamBigWigNew(char *fileOrUrl, struct annoAssembly *aa);
/* Create an annoStreamer (subclass) object from a file or URL. */

#endif//ndef ANNOSTREAMBIGWIG_H
