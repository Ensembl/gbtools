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

/* annoGratorQuery -- framework for integrating genomic annotations from many sources */

#ifndef ANNOGRATORQUERY_H
#define ANNOGRATORQUERY_H

#include "annoFormatter.h"
#include "annoGrator.h"

struct annoGratorQuery;
/* Representation of a complex query: multiple sources, each with its own filters,
 * output data and means of integration, aggregated and output by a formatter. */

struct annoGratorQuery *annoGratorQueryNew(struct annoAssembly *assembly,
					   struct annoStreamer *primarySource,
					   struct annoGrator *integrators,
					   struct annoFormatter *formatters);
/* Create an annoGratorQuery from all of its components.
 * integrators may be NULL.  All other inputs must be non-NULL. */

void annoGratorQuerySetRegion(struct annoGratorQuery *query, char *chrom, uint rStart, uint rEnd);
/* Set genomic region for query; if chrom is NULL, position is whole genome. */

void annoGratorQueryExecute(struct annoGratorQuery *query);
/* For each annoRow from query->primarySource, invoke integrators and
 * pass their annoRows to formatters. */

void annoGratorQueryFree(struct annoGratorQuery **pQuery);
/* Close and free all inputs and outputs; free self. */

#endif//ndef ANNOGRATORQUERY_H
