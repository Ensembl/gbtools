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

/* kxTok - quick little tokenizer for stuff first
 * loaded into memory.  Originally developed for
 * "Key eXpression" evaluator. 
 *
 * This file is copyright 2002 Jim Kent, but license is hereby
 * granted for all use - public, private or commercial. */

#ifndef KXTOK_H
#define KXTOK_H

enum kxTokType
    {
    kxtEnd,
    kxtString,
    kxtWildString,
    kxtEquals,
    kxtGT,      /* Greater Than */
    kxtGE,      /* Greater Than or Equal */
    kxtLT,      /* Less Than */
    kxtLE,      /* Less Than or Equal */
    kxtAnd,
    kxtOr,
    kxtXor,
    kxtNot,
    kxtOpenParen,
    kxtCloseParen,
    kxtAdd,
    kxtSub,
    kxtDiv,
    kxtMul,
    kxtDot,
    kxtMod,
    kxtPunct,
    };

struct kxTok
/* A key expression token.   Input text is tokenized
 * into a list of these. */
    {
    struct kxTok *next;
    enum kxTokType type;
    bool spaceBefore;	/* True if there is a space before */
    char string[1];  /* Allocated at run time */
    };

struct kxTok *kxTokenize(char *text, boolean wildAst);
/* Convert text to stream of tokens. If 'wildAst' is
 * TRUE then '*' character will be treated as wildcard
 * rather than multiplication sign. */

struct kxTok *kxTokenizeFancy(char *text, boolean wildAst,
			      boolean wildPercent, boolean includeHyphen);
/* Convert text to stream of tokens. If 'wildAst' is
 * TRUE then '*' character will be treated as wildcard
 * rather than multiplication sign.  
 * If wildPercent is TRUE then the '%' character will be treated as a 
 * wildcard (as in SQL) rather than a modulo (kxtMod) or percent sign.
 * If includeHyphen is TRUE then a '-' character in the middle of a String 
 * token will be treated as a hyphen (part of the String token) instead of 
 * a new kxtSub token. */

void kxTokIncludeQuotes(boolean val);
/* Pass in TRUE if kxTok should include quote characters in string tokens. */

#endif /* KXTOK_K */
