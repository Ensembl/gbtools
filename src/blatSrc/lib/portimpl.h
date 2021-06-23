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

/*****************************************************************************
 * Copyright (C) 2000 Jim Kent.  This source code may be freely used         *
 * for personal, academic, and non-profit purposes.  Commercial use          *
 * permitted only by explicit agreement with Jim Kent (jim_kent@pacbell.net) *
 *****************************************************************************/
/* Implement portable stuff.... */

/* There is one of the following structures for each web server
 * we support.  During run time looking at the environment variable
 * SERVER_SOFTWARE we decide which of these to use. */
struct webServerSpecific
    {
    char *name;

    /* Make a good name for a temp file. */
    void (*makeTempName)(struct tempName *tn, char *base, char *suffix);

    /* Return directory to look for cgi in. */
    char * (*cgiDir)();

#ifdef NEVER
    /* Return cgi suffix. */
    char * (*cgiSuffix)();
#endif /* NEVER */
    
    /* Return relative speed of CPU. (UCSC CSE 1999 FTP machine is 1.0) */
    double (*speed)();

    /* The relative path to trash directory for CGI binaries */
    char * (*trashDir)();

    };


extern struct webServerSpecific wssMicrosoftII, wssMicrosoftPWS, wssDefault,
	wssLinux, wssCommandLine, wssBrcMcw;

char *rTempName(char *dir, char *base, char *suffix);
/* Make a temp name that's almost certainly unique. */
