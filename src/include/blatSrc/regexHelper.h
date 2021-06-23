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

/* regexHelper: easy wrappers on POSIX Extended Regular Expressions (man 7 regex, man 3 regex) */

#ifndef REGEXHELPER_H
#define REGEXHELPER_H

#include "common.h"
#include <regex.h>

const regex_t *regexCompile(const char *exp, const char *description, int compileFlags);
/* Compile exp (or die with an informative-as-possible error message).
 * Cache pre-compiled regex's internally (so don't free result after use). */

boolean regexMatch(const char *string, const char *exp);
/* Return TRUE if string matches regular expression exp (case sensitive). */

boolean regexMatchNoCase(const char *string, const char *exp);
/* Return TRUE if string matches regular expression exp (case insensitive). */

boolean regexMatchSubstr(const char *string, const char *exp,
			 regmatch_t substrArr[], size_t substrArrSize);
/* Return TRUE if string matches regular expression exp (case sensitive);
 * regexec fills in substrArr with substring offsets. */

boolean regexMatchSubstrNoCase(const char *string, const char *exp,
			       regmatch_t substrArr[], size_t substrArrSize);
/* Return TRUE if string matches regular expression exp (case insensitive);
 * regexec fills in substrArr with substring offsets. */

#endif // REGEXHELPER_H
