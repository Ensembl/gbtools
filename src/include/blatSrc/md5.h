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

/* md5 calculating functions and the like.  Just wrappers for md5sum program */

#ifndef MD5_H
#define MD5_H

char *md5HexForFile(char * fileName);
/* Calculate md5 on file and return in hex format.  Use freeMem on result when done. */

char *md5HexForBuf(char *buf, size_t bufSize);
/* Return md5 sum of buffer. Use freeMem on result when done. */

char *md5HexForString(char *string);
/* Return md5 sum of zero-terminated string. Use freeMem on result when done. */

void md5ForFile(char * fileName, unsigned char md5[16]);
/* Return MD5 sum for file in md5 in binary rather than hex format. */

void md5HexToMd5(char hexIn[32], unsigned char md5Out[16]);
/* Convert hexadecimal representation of md5 back to binary */

char *md5ToHex(unsigned char md5[16]);
/* Convert binary representation of md5 to hex string. Do a freeMem on result when done. */

void md5HexToMd5(char hexIn[32], unsigned char md5Out[16]);
/* Convert hexadecimal representation of md5 back to binary */

struct hash *md5FileHash(char *fileName);
/* Read md5sum file and return a hash keyed by file names with md5sum values. */

#endif /* MD5_H */


