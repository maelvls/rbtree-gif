/* ===========================================================
 *
 *       Filename:  rbtree.h
 *
 *    Description:  
 *
 *        Created:  03-04-2013 14:15
 *
 *         Author:  Mael Valais 
 *         Mail  :	mael.valais@univ-tlse3.fr
 *
 * =========================================================== */

#ifndef __rbtree_h__
#define __rbtree_h__

#include <stdlib.h>
#include "queue.h"

typedef struct _rbtree *RBTree;
typedef struct _node *Node;

#define rbfirst(tree)		((tree)->root->left)
#define rbgrandpa(node)		((node)->father->father)
#define rbExists(node) 		((node)->father != (node))
#define rbIsLeaf(node)		((node)->left == (node)->right)

RBTree rbtreeCreate(int (*cmp)(const void*, const void*), int (*equal)(const void*, const void*));

int rbtreeEmpty(RBTree tree);

void rbtreeInsert(RBTree tree, void* data);
void rbtreeClassicInsertDebug(RBTree tree, void* data);

void rbtreeToDot(RBTree tree, const char* racine, const char* dossier);
void rbtreeMapDebug(RBTree tree); 

void rbtreeRemove(RBTree tree, void* data);

#endif /* !__rbtree_h__ */

