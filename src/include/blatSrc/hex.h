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


#ifndef HEX_H
#define HEX_H

char nibbleToHex(char n);
/* convert nibble to hexidecimal character. 0 <= n <= 15. */

void byteToHex(unsigned char n, char *hex);
/* convert byte to two hexidecimal characters. 0 <= n <= 255. */

char *byteToHexString(unsigned char n);
/* convert byte to hexidecimal string. 0 <= n <= 255. */

void hexBinaryString(unsigned char *in, int inSize, char *out, int outSize);
/* Convert possibly long binary string to hex string.
 * Out size needs to be at least 2x inSize+1 */

/* Reverse Functions */

char hexToNibble(char n);
/* convert hexidecimal character to nibble. 0-9a-f. */

unsigned char hexToByte(char *hex);
/* convert byte to hexidecimal characters. 0 <= n <= 255. */

#endif /* HEX_H */

