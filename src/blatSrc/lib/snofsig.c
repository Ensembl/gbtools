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

/* snofSig - signature (first 16 bytes) of a snof format file. 
 *
 * This file is copyright 2002 Jim Kent, but license is hereby
 * granted for all use - public, private or commercial. */

#include "common.h"
#include "snofmake.h"


static int ixSig[4] = {0x693F8ED1, 0x7EDA1C32, 0x4BA58983, 0x277CB89C,};

void snofSignature(char **rSig, int *rSigSize)
/* Return signature. */
{
*rSig = (char *)ixSig;
*rSigSize = sizeof(ixSig);
}

boolean isSnofSig(void *sig)
/* Return true if sig is right. */
{
return memcmp(sig, ixSig, sizeof(ixSig)) == 0;
}

