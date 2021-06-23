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

/* jsonWrite - Helper routines for writing out JSON. */

#ifndef JSONWRITE_H
#define JSONWRITE_H

struct jsonWrite
/* Object to help output JSON */
     {
     struct jsonWrite *next;
     struct dyString *dy;	/* Most of this module is building json text in here */
     bool objStack[128];	/* We need stack deep enough to handle nested objects and lists */
     int stackIx;		/* Current index in stack */
     };

struct jsonWrite *jsonWriteNew();
/* Return new empty jsonWrite struct. */

void jsonWriteFree(struct jsonWrite **pJw);
/* Free up a jsonWrite object. */

void jsonWriteTag(struct jsonWrite *jw, char *var);
/* Print out quoted tag followed by colon. Print out preceding comma if need be.  */

void jsonWriteEndLine(struct jsonWrite *jw);
/* Write comma if in middle, and then newline regardless. */

void jsonWriteString(struct jsonWrite *jw, char *var, char *string);
/* Print out "var": "val".  If var is NULL then just print out "val" */

void jsonWriteDateFromUnix(struct jsonWrite *jw, char *var, long long unixTimeVal);
/* Add "var": YYYY-MM-DDT-HH:MM:SSZ given a Unix time stamp. Var may be NULL. */

void jsonWriteNumber(struct jsonWrite *jw, char *var, long long val);
/* print out "var": val as number. Var may be NULL. */

void jsonWriteLink(struct jsonWrite *jw, char *var, char *objRoot, char *name);
/* Print out the jsony type link to another object.  objRoot will start and end with a '/'
 * and may have additional slashes in this usage. Var may be NULL. */

void jsonWriteLinkNum(struct jsonWrite *jw, char *var, char *objRoot, long long id);
/* Print out the jsony type link to another object with a numerical id.  objRoot will start 
 * and end with a '/' and may have additional slashes in this usage. Var may be NULL */

void jsonWriteListStart(struct jsonWrite *jw, char *var);
/* Start an array in JSON. Var may be NULL */

void jsonWriteListEnd(struct jsonWrite *jw);
/* End an array in JSON */

void jsonWriteObjectStart(struct jsonWrite *dy);
/* Print start of object */

void jsonWriteObjectEnd(struct jsonWrite *jw);
/* End object in JSON */

#endif /* JSONWRITE_H */
