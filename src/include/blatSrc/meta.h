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

/* metaRa - stuff to parse and interpret a genome-hub meta.txt file, which is in 
 * a hierarchical ra format.  That is something like:
 *     meta topLevel
 *     cellLine HELA
 *
 *         meta midLevel
 *         target H3K4Me3
 *         antibody abCamAntiH3k4me3
 *       
 *            meta lowLevel
 *            fileName hg19/chipSeq/helaH3k4me3.narrowPeak.bigBed
 * The file is interpreted so that lower level stanzas inherit tags from higher level ones.
 */

#ifndef META_H
#define META_H

struct metaTagVal
/* A tag/value pair. */
    {
    struct metaTagVal *next;	/* Next in list. */
    char *tag;	/* Tag name. */
    char *val;	/* Tag value. */
    };

struct metaTagVal *metaTagValNew(char *tag, char *val);
/* Create new meta tag/val */

void metaTagValFree(struct metaTagVal **pMtv);
/* Free up metaTagVal. */

void metaTagValFreeList(struct metaTagVal **pList);
/* Free a list of dynamically allocated metaTagVal's */

int metaTagValCmp(const void *va, const void *vb);
/* Compare to sort based on tag name . */

struct meta
/* A node in the metadata tree */
    {
    struct meta *next;  /* Pointer to next younger sibling. */
    struct meta *children;	/* Pointer to eldest child. */
    struct meta *parent;	/* Pointer to parent. */
    char *name;		    /* Same as val of meta tag. Not allocated here. */
    struct metaTagVal *tagList;	/* All tags, including the "meta" one. */
    int indent;                 /* Indentation level - generally only set if read from file. */
    };

struct meta *metaLoadAll(char *fileName, char *keyTag, char *parentTag,
    boolean ignoreOtherStanzas, boolean ignoreIndent);
/* Loads in all ra stanzas from file and turns them into a list of meta, some of which
 * may have children.  The keyTag parameter is optional.  If non-null it should be set to
 * the tag name that starts a stanza.   If null, the first tag of the first stanza will be used.
 * The parentTag if non-NULL will be a tag name used to define the parent of a stanza.
 * The ignoreOtherStanzas flag if set will ignore stanzas that start with other tags.  
 * If not set the routine will abort on such stanzas.  The ignoreIndent if set will
 * use the parentTag (which must be set) to define the hierarchy.  Otherwise the program
 * will look at the indentation, and if there is a parentTag complain about any
 * disagreements between indentation and parentTag. */

void metaFree(struct meta **pMeta);
/* Free up memory associated with a meta. */

void metaFreeForest(struct meta **pForest);
/* Free up all metas in forest and their children. */ 

void metaFreeList(struct meta **pList);
/* Free a list of dynamically allocated meta's. Use metaFreeForest to free children too. */

#define META_DEFAULT_INDENT 4	/* Default size for meta indentation */

void metaWriteAll(struct meta *metaList, char *fileName, int indent, boolean withParent);
/* Write out metadata, including children, optionally adding meta tag.   By convention
 * for out meta.txt/meta.ra files, indent is 4, withParent is FALSE. */

char *metaLocalTagVal(struct meta *meta, char *tag);
/* Return value of tag found in this node, not going up to parents. */

char *metaTagVal(struct meta *meta, char *tag);
/* Return value of tag found in this node or if its not there in parents.
 * Returns NULL if tag not found. */

void metaAddTag(struct meta *meta, char *tag, char *val);
/* Add tag/val to meta. */

void metaSortTags(struct meta *meta);
/* Do canonical sort so that the first tag stays first but the
 * rest are alphabetical. */

struct hash *metaHash(struct meta *forest);
/* Return hash of meta at all levels of heirarchy keyed by meta value. */

#endif /* META_H */
