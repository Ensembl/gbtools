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

/* scoreWindow - find window with most matches to a given char */

/* simple program to find max scoring window representing string of char c in a string s of size size */
/* index of max score is returned , match and misMatch are the scores to assign, suggested defaults are match=1 and misMatch=1*/
/* when used for scoring polyA tails, set c='A' for positive strand  or c='T' for neg strand */
/* start, end are returned pointing to the start and end of the highest scoring window in s */
int scoreWindow(char c, char *s, int size, int *score, int *start, int *end, int match, int misMatch);
