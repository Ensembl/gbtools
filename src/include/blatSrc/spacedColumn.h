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

/* spacedColumn - stuff to handle parsing text files where fields are
 * fixed width rather than tab delimited. */

#ifndef SPACEDCOLUMN_H
#define SPACEDCOLUMN_H

struct spacedColumn
/* Specs on a column. */
    {
    struct spacedColumn *next;
    int start;	/* Starting index. */
    int size;	/* Size of column. */
    };

#define spacedColumnFreeList slFreeList

struct spacedColumn *spacedColumnFromWidthArray(int array[], int size);
/* Return a list of spaced columns corresponding to widths in array.
 * The final char in each column should be whitespace. */

struct spacedColumn *spacedColumnFromSample(char *sample);
/* Return spaced column list from a sample line , which is assumed to
 * have no spaces except between columns */

struct spacedColumn *spacedColumnFromSizeCommaList(char *commaList);
/* Given an comma-separated list of widths in ascii, return
 * a list of spacedColumns. */

struct spacedColumn *spacedColumnFromLineFile(struct lineFile *lf);
/* Scan through lineFile and figure out column spacing. Assumes
 * file contains nothing but columns. */

struct spacedColumn *spacedColumnFromFile(char *fileName);
/* Read file and figure out where columns are. */

int spacedColumnBiggestSize(struct spacedColumn *colList);
/* Return size of biggest column. */

boolean spacedColumnParseLine(struct spacedColumn *colList, 
	char *line, char *row[]);
/* Parse line into row according to colList.  This will
 * trim leading and trailing spaces. It will write 0's
 * into line.  Returns FALSE if there's a problem (like
 * line too short.) */

#endif /* SPACEDCOLUMN_H */
