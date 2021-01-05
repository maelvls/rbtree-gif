/*
 * A red-black tree data structure.
 */

#include <stdbool.h>
#include <stdio.h>

#include "queue.h"

#define rb_first(tree) ((tree)->root->left)
#define rb_grandparent(node) ((node)->father->father)
#define rb_exists(node) ((node)->father != (node))
#define rb_is_leaf(node) ((node)->left == (node)->right)

struct rbtree *rbtree_new(int (*cmp)(const void *, const void *), int (*equal)(const void *, const void *));

bool rbtree_empty(struct rbtree *tree);

void rbtree_insert(struct rbtree *tree, void *data);
void rbtree_classic_insert_debug(struct rbtree *tree, void *data);

void rbtree_to_dot(struct rbtree *tree, const char *racine, const char *dossier);
void rbtree_map_debug(struct rbtree *tree);

void rbtree_remove(struct rbtree *tree, void *data);
