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

/* Quoted Printable encoding and decoding.
 * by Galt Barber */

#ifndef QUOTEDP_H
#define QUOTEDP_H

char *quotedPrintableEncode(char *input);
/* Use Quoted-Printable standard to encode a string.
 */

boolean quotedPCollapse(char *line);
/* Use Quoted-Printable standard to decode a string.
 * Return true if the line does not end in '='
 * which indicate continuation. */

char *quotedPrintableDecode(char *input);
/* Use Quoted-Printable standard to decode a string.  Return decoded
 * string which will be freeMem'd. */

#endif /* QUOTEDP_H */

