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

/* Base64 encoding and decoding.
 * by Galt Barber */

#ifndef BASE64_H
#define BASE64_H

#define B64CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

char *base64Encode(char *input, size_t inplen);
/* Use base64 to encode a string.  Returns one long encoded
 * string which need to be freeMem'd. Note: big-endian algorithm.
 * For some applications you may need to break the base64 output
 * of this function into lines no longer than 76 chars.
 */

boolean base64Validate(char *input);
/* Return true if input is valid base64.
 * Note that the input string is changed by 
 * eraseWhiteSpace(). */

char *base64Decode(char *input, size_t *returnSize);
/* Use base64 to decode a string.  Return decoded
 * string which will be freeMem'd. Note: big-endian algorithm.
 * Call eraseWhiteSpace() and check for invalid input 
 * before passing in input if needed.  
 * Optionally set retun size for use with binary data.
 */

#endif /* BASE64_H */

