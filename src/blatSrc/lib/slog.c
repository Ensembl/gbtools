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

/* slog - fixed point scaled logarithm stuff. 
 *
 * This file is copyright 2002 Jim Kent, but license is hereby
 * granted for all use - public, private or commercial. */

#include "common.h"
#include "slog.h"


double fSlogScale = 8192.0;	/* Convert to fixed point by multiplying by this. */
double invSlogScale = 0.0001220703125; /* Conver back to floating point with this. */

int slog(double val)
/* Return scaled log. */
{
return (round(fSlogScale*log(val)));
}

int carefulSlog(double val)
/* Returns scaled log that makes sure there's no int overflow. */
{
if (val < 0.0000001)
    val = 0.0000001;
return slog(val);
}

double invSlog(int scaledLog)
/* Inverse of slog. */
{
return exp(scaledLog*invSlogScale);
}

