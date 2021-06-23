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

/* seqOut - stuff to output sequences and alignments in web 
 * or ascii viewable form. */


struct cfm
/* Colored web character output formatter. */
    {
    int wordLen;	/* Number of characters between spaces (often 10) */
    int lineLen;        /* Number of characters between lines (often 50) */
    int inWord, inLine; /* Position in word and line. */
    bool lineNumbers;   /* True if write position at end of line. */
    bool countDown;     /* True if want numbers counting down. */
    long charCount;     /* Number of characters written total. */
    FILE *out;          /* File to write to. */
    int numOff;         /* Number to start with. */
    int color;          /* Cache last color here. */
    bool underline;	/* Underline? */
    bool bold;		/* Font in bold. */
    bool italic;	/* Italic? */
    };

struct cfm *cfmNew(int wordLen, int lineLen, 
	boolean lineNumbers, boolean countDown, FILE *out, int numOff);
/* Set up colored sequence formatting for html. */

void cfmOut(struct cfm *cfm, char c, int color);
/* Write out a byte, and depending on color formatting extras  */

void cfmOutExt(struct cfm *cfm, char c, int color, boolean underline, boolean bold, boolean italic);
/* Write out a byte, and formatting extras  */

void cfmFree(struct cfm **pCfm);
/* Finish and free up cfm formatting job. */

enum seqOutColor
/* Symbolic color for sequence output. */
    {
    socBlack = 0,		/* Not aligning. */
    socBlue = 1,		/* Aligning. */
    socBrightBlue = 2,		/* End of an aligning block. */
    socRed = 3, 		/* Aligning UTR. */
    socOrange = 4,		/* End of an aligning UTR block. */
    };
extern int seqOutColorLookup[];		/* Converts these to html format colors. */

struct baf
/* Block allignment formatter. */
    {
    char nChars[256];
    char hChars[256];
    int cix;
    int nLineStart;
    int hLineStart;
    int nCurPos;
    int hCurPos;
    DNA *needle, *haystack;
    int nNumOff, hNumOff;
    FILE *out;
    int lineSize;
    bool hCountDown;     /* True if want numbers counting down. */
    bool isTrans;	 /* True if haystack is translated. */
    bool nCountDown;	 /* True if want needle numbers counting down. */
    };

void bafInit(struct baf *baf, DNA *needle, int nNumOff, boolean nCountDown,
	DNA *haystack, int hNumOff, boolean hCountDown, 
	FILE *out, int lineSize, boolean isTrans);
/* Initialize block alignment formatter. */

void bafSetAli(struct baf *baf, struct ffAli *ali);
/* Set up block formatter around an ffAli block. */

void bafSetPos(struct baf *baf, int nStart, int hStart);
/* Set up block formatter starting at nStart/hStart. */

void bafStartLine(struct baf *baf);
/* Set up block formatter to start new line at current position. */

void bafWriteLine(struct baf *baf);
/* Write out a line of an alignment (which takes up
 * three lines on the screen. */

void bafOut(struct baf *baf, char n, char h);
/* Write a pair of character to block alignment. */

void bafFlushLineNoHr(struct baf *baf);
/* Write out alignment line if it has any characters in it (no <HR>). */

void bafFlushLine(struct baf *baf);
/* Write out alignment line if it has any characters in it, and an <HR>. */

