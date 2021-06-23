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

/* Handle escaping for XML files.  Deal with things like
 * &amp; and &quot. */

/* Copyright (C) 2011 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */

#include "common.h"
#include "hash.h"
#include "xmlEscape.h"


struct hash *xmlEscapeSymHash()
/* Return hash of predefined xml character symbols to lookup. */
{
struct hash *symHash = newHash(6);
hashAdd(symHash, "lt", "<");
hashAdd(symHash, "gt", ">");
hashAdd(symHash, "amp", "&");
hashAdd(symHash, "apos", "'");
hashAdd(symHash, "quot", "\"");
return symHash;
}

void xmlEscapeBytesToFile(unsigned char *buffer, int len, FILE *f)
/* Write buffer of given length to file, escaping as need be. */
{
unsigned char c;
int i;
for (i=0; i<len; ++i)
    {
    c = buffer[i];
    if (isalnum(c))
        fputc(c, f);
    else
        {
	switch (c)
	    {
	    case '&':
	        fputs("&amp;", f);
		break;
	    case '\'':
	        fputs("&apos;", f);
		break;
	    case '"':
	        fputs("&quot;", f);
		break;
	    case '<':
	        fputs("&lt;", f);
		break;
	    case '>':
	        fputs("&gt;", f);
		break;
	    case ' ':
	    case '-':
	    case '\t':
	    case '\n':
	    case ',':
	    case '.':
	    case ';':
	    case ':':
	    case '(':
	    case ')':
	    case '[':
	    case ']':
	    case '#':
	    case '/':
	        fputc(c, f);
		break;
	    default:
	        fprintf(f, "&#%d;", c);
		break;
	    }
	}
    }
}

void xmlEscapeStringToFile(char *s, FILE *f)
/* Write escaped zero-terminated string to file. */
{
int len = strlen(s);
xmlEscapeBytesToFile((unsigned char *)s, len, f);
}
