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

/* annoFormatTab -- collect fields from all inputs and print them out, tab-separated. */

#ifndef ANNOFORMATTAB_H
#define ANNOFORMATTAB_H

#include "annoFormatter.h"

struct annoFormatter *annoFormatTabNew(char *fileName);
/* Return a formatter that will write its tab-separated output to fileName. */

#endif//ndef ANNOFORMATTAB_H
