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

/* phlyogenetic trees */


#ifndef PHYLOTREE_H
#define PHYLOTREE_H

#include "linefile.h"

struct phyloName
{
    char *name;		/* name of this node */
    double length;	/* length of the branch to this node */
};

struct phyloTree
{
    struct phyloTree *next;		/* next in single linked list */
    struct phyloTree *parent;		/* the parent of this node */
    struct phyloTree **edges;
    struct phyloName *ident;		/* the name and branch length */
    struct phyloTree *mark;		/* probably the favorite child */
    bool isDup;				/* is this a duplication node */
    int numEdges;
    int allocedEdges;

    void *priv;
};

extern struct phyloTree *phyloOpenTree(char *fileName);
/* opens an NH tree file */

extern struct phyloTree *phyloReadTree(struct lineFile *lf);
/* reads a phyloTree from lineFile (first line only) */

extern struct phyloTree *phyloParseString(char *string);
/* build a phyloTree from a string */

extern void phyloPrintTree( struct phyloTree *,FILE *f);
/* print a phyloTree to f in Newick format */

extern void phyloDebugTree( struct phyloTree *,FILE *f);
/* print a phyloTree to f */

extern char *phyloFindPath(struct phyloTree *tree, char *ref, char *cross);
/* find the shortest path from ref to cross (returns a list
 * of the node names separated by spaces */

extern char *phyloNodeNames(struct phyloTree *tree);
/* return list of all the node names separated by spaces */

extern struct phyloTree *phyloFindName( struct phyloTree *tree,char *name );
/* find the node with this name */

extern struct phyloTree *phyloReRoot(struct phyloTree *inTree);
/* return a tree whose root is inTree and what were parents are now "right" children */

extern void phyloDeleteEdge(struct phyloTree *tree, struct phyloTree *edge);
/* delete an edge from a node.  Aborts on error */

extern struct phyloTree *phyloAddEdge(struct phyloTree *parent, struct phyloTree *child);
/* add an edge to a phyloTree node */

extern void phyloClearTreeMarks(struct phyloTree *tree);
/* clear the favorite child marks */

extern struct phyloTree *phyloFindMarkUpTree(struct phyloTree *tree);
/* find a marked node somewhere above this node */

extern void phyloMarkUpTree(struct phyloTree *tree);
/* mark all the nodes from this one up to the top of the tree */

extern void phyloPrintTreeNoDups( struct phyloTree *tree,FILE *f);
/* print out phylogenetic tree in Newick format (only speciation nodes) */

extern int phyloCountLeaves( struct phyloTree *tree);

#endif
