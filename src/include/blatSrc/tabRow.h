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

/* tabRow - a row from a database or a tab-separated file held in
 * memory.   Just a light wrapper around an array of strings. 
 * Also some routines to convert slLines to tabRows. */

#ifndef TABROW_H
#define TABROW_H

struct tabRow
/* A parsed out tableRow. */
    {
    struct tabRow *next;
    int colCount;
    char *columns[1];
    };

struct tabRow *tabRowNew(int colCount);
/* Return new row. */

int tabRowMaxColCount(struct tabRow *rowList);
/* Return largest column count */

struct tabRow *tabRowByWhite(struct slName *lineList, char *fileName,
	boolean varCol);
/* Convert lines to rows based on spaces.  If varCol is TRUE then not
 * all rows need to have same number of columns. */

struct tabRow *tabRowByChar(struct slName *lineList, char c, char *fileName,
	boolean varCol);
/* Convert lines to rows based on character separation.  If varCol is TRUE then not
 * all rows need to have same number of columns. */

struct tabRow *tabRowByFixedOffsets(struct slName *lineList, struct slInt *offList,
	char *fileName);
/* Return rows parsed into fixed width fields whose starts are defined by
 * offList. */

struct tabRow *tabRowByFixedGuess(struct slName *lineList, char *fileName);
/* Return rows parsed into fixed-width fields. */

struct slInt *tabRowGuessFixedOffsets(struct slName *lineList, char *fileName);
/* Return our best guess list of starting positions for space-padded fixed
 * width fields. */

#endif /* TABROW_H */

