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

/* histogram function for data array in memory	*/

#if !defined(HISTOGRAM_H)
#define HISTOGRAM_H

#define	DEFAULT_BIN_COUNT	26
/*	ARBITRARY default choice of # of bins	*/

struct histoResult {
    struct histoResult *next;	/*	might be a list of them	*/
    float binSize;		/*	size of a bin	*/
    unsigned binCount;		/*	number of bins	*/
    unsigned count;		/*	number of values	*/
    float binZero;		/*	bin 0 starts at this value	*/
    unsigned *binCounts;	/*	array binCounts[binCount]	*/
    float *pValues;		/*	array pValues[binCount]		*/
};

void freeHistoGram(struct histoResult **histoResults);
/*      free the histoResults list	*/

struct histoResult *histoGram(float *values, size_t N, float binSize,
	unsigned binCount, float minValue, float min, float max,
	struct histoResult *accumHisto);
/*	construct histogram of data in values[N] array.	*/
#endif
