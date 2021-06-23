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

/* gfNet.c - Network dependent stuff for blat server. */

/* Copyright (C) 2011 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */

#include "common.h"
#include "errAbort.h"
#include "genoFind.h"
#include "net.h"


int gfConnect(char *hostName, char *portName)
/* Start connection with server. */
{
/* Connect to server. */
int sd = netConnect(hostName, atoi(portName));
if (sd < 0)
    {
    errnoAbort("Sorry, the BLAT/iPCR server seems to be down.  Please try "
               "again later.");
    }
return sd;
}

