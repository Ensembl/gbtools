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

/* knetUdc -- install udc i/o functions in knetfile interface in Heng Li's samtools lib. */
/* As of 2/23/10, the KNETFILE_HOOKS extension is a UCSC-local modification of samtools. */

#ifndef KNETUDC_H
#define KNETUDC_H

void knetUdcInstall();
/* install udc i/o functions in knetfile interface in Heng Li's samtools lib. */

#endif//ndef KNETUDC_H
