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

/* aliType - some definitions for type of alignment. */

/* Copyright (C) 2011 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */
#include "common.h"
#include "aliType.h"


char *gfTypeName(enum gfType type)
/* Return string representing type. */
{
if (type == gftDna) return "DNA";
if (type == gftRna) return "RNA";
if (type == gftProt) return "protein";
if (type == gftDnaX) return "DNAX";
if (type == gftRnaX) return "RNAX";
internalErr();
return NULL;
}

enum gfType gfTypeFromName(char *name)
/* Return type from string. */
{
if (sameWord(name, "DNA")) return gftDna;
if (sameWord(name, "RNA")) return gftRna;
if (sameWord(name, "protein")) return gftProt;
if (sameWord(name, "prot")) return gftProt;
if (sameWord(name, "DNAX")) return gftDnaX;
if (sameWord(name, "RNAX")) return gftRnaX;
errAbort("Unknown sequence type '%s'", name);
return 0;
}

