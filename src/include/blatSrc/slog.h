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

/* slog - fixed point scaled logarithm stuff. */
#ifndef SLOG_H
#define SLOG_H

extern double fSlogScale;	/* Convert to fixed point by multiplying by this. */
extern double invSlogScale;	/* To convert back to floating point use this. */

int slog(double val);
/* Return scaled log. */

int carefulSlog(double val);
/* Returns scaled log that makes sure there's no int overflow. */

double invSlog(int scaledLog);
/* Inverse of slog. */

#endif /* SLOG_H */

