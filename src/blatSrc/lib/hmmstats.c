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

/* hmmstats.c - Stuff for doing statistical analysis in general and 
 * hidden Markov models in particular. 
 *
 * This file is copyright 2002 Jim Kent, but license is hereby
 * granted for all use - public, private or commercial. */

#include "common.h"
#include "hmmstats.h"


int scaledLog(double val)
/* Return scaled log of val. */
{
return round(logScaleFactor * log(val));
}

double oneOverSqrtTwoPi = 0.39894228;

double simpleGaussean(double x)
/* Gaussean distribution with standard deviation 1 and mean 0. */
{
return oneOverSqrtTwoPi * exp(-0.5*x*x );
}

double gaussean(double x, double mean, double sd)
/* Gaussean distribution with mean and standard deviation at point x  */
{
x -= mean;
x /= sd;
return oneOverSqrtTwoPi * exp(-0.5*x*x) / sd;
}

double calcVarianceFromSums(double sum, double sumSquares, bits64 n)
/* Calculate variance. */
{
double var = sumSquares - sum*sum/n;
if (n > 1)
    var /= n-1;
return var;
}

double calcStdFromSums(double sum, double sumSquares, bits64 n)
/* Calculate standard deviation. */
{
return sqrt(calcVarianceFromSums(sum, sumSquares, n));
}


