#include <stdlib.h>
// #include <stdio.h>
// #include <assert.h>

#include "utils_ds/rb_tree.h"

#define RBT_RED 0
#define RBT_BLACK 1

// struct RBT_iterator {
//     void* key;
// };

typedef struct RBT_node {
    // void* key;
    RBT_iterator it;
    char color;
    struct RBT_node* parent;
    struct RBT_node* left; // left child
    struct RBT_node* right; // right child
} RBT_node;
/*  
    A red black tree statisfies the following properties.
    1. Every node is either red or black.
    2. The root is black.
    3. Every leaf (NIL) is black.
    4. If a node is red, then both its children are black.
    5. For each node, all simple paths from the node to descendant leaves contain the
       same number of black nodes.
*/
struct RB_tree {
    RBT_node* root;
    cmp_key cmp_keys;
};

// typedef struct RBT_iterator_pair {

// } RBT_iterator_pair;

static RBT_node sentinel = {
    .it = {.key = NULL},
    .color = RBT_BLACK,
    .parent = &sentinel,
    .left = &sentinel,
    .right = &sentinel
};

static RBT_node* NIL = &sentinel;

RB_tree* rbt_create(cmp_key cmp) {
    RB_tree* tree = (RB_tree*)malloc(sizeof(*tree));
    if (tree == NULL) {
        return NULL;
    }

    tree->root = NIL;
    tree->cmp_keys = cmp;

    return tree;
}


static RBT_node* node_create(void* key) {
    RBT_node* node = (RBT_node*)malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    // node->key = key;
    node->it.key = key;
    node->color = RBT_RED;
    node->parent = NIL;
    node->left = NIL;
    node->right = NIL;

    return node;
}

static void node_destroy(RBT_node* node) {
    free(node);
}

static void destroy_tree(RBT_node* root) {
    if (root->left != NIL) {
        destroy_tree(root->left);
    }
    if (root->right != NIL) {
        destroy_tree(root->right);
    }

    node_destroy(root);
}

void rbt_destroy(RB_tree* t) {
    if (t->root != NIL) {
        destroy_tree(t->root);
    }

    free(t);
}

// static RBT_node* rbt_find_node(RB_tree* t, void* key) {
//     RBT_node* x = t->root;
//     int cmp_res;
//     while (x != NIL && (cmp_res = t->cmp_keys(key, x->it.key)) != 0) {
//         if (cmp_res < 0) {
//             x = x->left;
//         } else {
//             x = x->right;
//         }
//     }

//     return x;
// }

static RBT_node* rbt_find_first_node_with_key(RB_tree* t, void* key) {
    RBT_node* x = t->root;
    RBT_node* ret = NIL;
    int cmp_res;
    while (x != NIL) {
        cmp_res = t->cmp_keys(key, x->it.key);
        if (cmp_res < 0) {
            x = x->left;
        } else if (cmp_res > 0) {
            x = x->right;
        } else {
            ret = x;
            x = x->left;
        }
    }

    return ret;
}

RBT_iterator* rbt_find(RB_tree* t, void* key) {
    // RBT_node* x = rbt_find_node(t, key);
    RBT_node* x = rbt_find_first_node_with_key(t, key);
    return (x == NIL) ? NULL : &x->it;
    // return x->it.key;
}

static RBT_node* rbt_first_node(RBT_node* x) {
    while (x->left != NIL) {
        x = x->left;
    }

    return x;
}

RBT_iterator* rbt_first(RB_tree* rbt) {
    RBT_node* f = rbt_first_node(rbt->root);
    // return (f == NIL) ? NULL : f->key;
    // return f->it.key;
    return (f == NIL) ? NULL : &f->it;
}

static RBT_node* rbt_last_node(RBT_node* x) {
    while (x->right != NIL) {
        x = x->right;
    }

    return x;
}

RBT_iterator* rbt_last(RB_tree* rbt) {
    RBT_node* l = rbt_last_node(rbt->root);
    // return (l == NIL) ? NULL : l->key;
    // return l->it.key;
    return (l == NIL) ? NULL : &l->it;
}

RBT_iterator* rbt_find_before(RB_tree* rbt, void* key) {
    RBT_node* x = rbt->root, *ret = NULL;
    int cmp_res;
    while (x != NIL) {
        cmp_res = rbt->cmp_keys(key, x->it.key);
        if (cmp_res <= 0) {
            x = x->left;
        } else {
            ret = x;
            x = x->right;
        }
    }

    return (ret == NULL) ? NULL : &ret->it;
}

RBT_iterator* rbt_find_after(RB_tree* rbt, void* key) {
    RBT_node* x = rbt->root, *ret = NULL;
    int cmp_res;
    while (x != NIL) {
        cmp_res = rbt->cmp_keys(key, x->it.key);
        if (cmp_res < 0) {
            ret = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }

    return (ret == NULL) ? NULL : &ret->it;
}

static RBT_node* rbt_node_next(RBT_node* x) {
    if (x->right != NIL) {
        return rbt_first_node(x->right);
    }
    RBT_node* y = x->parent;
    while (y != NIL && x == y->right) {
        x = y;
        y = y->parent;
    }

    return y;
}

static RBT_node* rbt_node_prev(RBT_node* x) {
    if (x->left != NIL) {
        return rbt_last_node(x->left);
    }
    RBT_node* y = x->parent;
    while (y != NIL && x == y->left) {
        x = y;
        y = y->parent;
    }

    return y;
}

/*
    Get An interator which contains the next key of the given iterator.
    return the iterator,
    NULL on error or if the next key doesn't exist in the tree.
*/
RBT_iterator* rbt_iterator_next(RB_tree* rbt, RBT_iterator* iter) {
    RBT_node* node = rbt_node_next((RBT_node*)iter);
    return (node == NIL) ? NULL : &node->it;
}

/*
    Get An interator which contains the prev key of the given iterator.
    return the iterator,
    NULL on error or if the prev key doesn't exist in the tree.
*/
RBT_iterator* rbt_iterator_prev(RB_tree* rbt, RBT_iterator* iter) {
    RBT_node* node = rbt_node_prev((RBT_node*)iter);
    return (node == NIL) ? NULL : &node->it;
}

static void rbt_left_rotate(RB_tree* t, RBT_node* x) {
    RBT_node* y = x->right;
    x->right = y->left; // turn y's left subtree into x's right subtree
    if (y->left != NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NIL) {
        t->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x; // put x on y's left
    x->parent = y;
}

static void rbt_right_rotate(RB_tree* t, RBT_node* x) {
    RBT_node* y = x->left;
    x->left = y->right; // turn y's left subtree into x's right subtree
    if (y->right != NIL) {
        y->right->parent = x;
    }
    
    y->parent = x->parent;
    if (x->parent == NIL) {
        t->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x; // put x on y's left
    x->parent = y;
}

/*
    Restore the red-black tree properties.

    The inserted node is colored red. This may
    violate some red-black tree properties(2 and 4).
    This function fix the violations by coloring some nodes
    and/or performing rotations on subtrees which moves up
    the pointer z

*/
static void rbt_insert_fixup(RB_tree* t, RBT_node* z) {
    RBT_node* y;
    while (z->parent->color == RBT_RED) {
    // Restore the prorperty 4
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            // case 1: z's uncle is red
            if (y->color == RBT_RED) {
                z->parent->color = RBT_BLACK;
                y->color = RBT_BLACK;
                z->parent->parent->color = RBT_RED;

                z = z->parent->parent;
            } else {
                // case 2: z's uncle is black and z is a right child
                if (z == z->parent->right) {
                    z = z->parent;
                    rbt_left_rotate(t, z);
                }
                // case 3: z's uncle is black and z is a left child
                z->parent->color = RBT_BLACK;
                z->parent->parent->color = RBT_RED;
                rbt_right_rotate(t, z->parent->parent);
            }
        } else { 
        // symmetric to the upper code
            y = z->parent->parent->left;

            if (y->color == RBT_RED) {
                z->parent->color = RBT_BLACK;
                y->color = RBT_BLACK;
                z->parent->parent->color = RBT_RED;

                z = z->parent->parent;
            } else {

                if (z == z->parent->left) {
                    z = z->parent;
                    rbt_right_rotate(t, z);
                }

                z->parent->color = RBT_BLACK;
                z->parent->parent->color = RBT_RED;
                rbt_left_rotate(t, z->parent->parent);
            }
        }
    }

    t->root->color = RBT_BLACK;
}

/*
    Insert the key
    return an iterator to the key, or NULL on error
*/
RBT_iterator* rbt_insert(RB_tree* t, void* key) {
    RBT_node* z = node_create(key);
    if (z == NULL) {
        return NULL;
    }

    RBT_node* y = NIL;
	RBT_node* x = t->root;

	while (x != NIL) {
        y = x;
        if (t->cmp_keys(z->it.key, x->it.key) < 0) {
            x = x->left;
        }
        else {
            x = x->right;
        }
	}
    z->parent = y;

    if (y == NIL) {
        t->root = z;
    } else if (t->cmp_keys(z->it.key, y->it.key) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = NIL;
    z->right = NIL;
    z->color = RBT_RED;

    rbt_insert_fixup(t, z);

	// return z->it.key;
    return &z->it;
}

static void rbt_transplant(RB_tree* t, RBT_node* u, RBT_node* v) {
    if (u->parent == NIL) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

/*  
    Restore the red-black tree properties.

    When this functions is called in the rbt_delete function,
    the x has been moved into node y's original positions
    which has been removed from the tree or moved within the tree.
*/
static void rbt_delete_fixup(RB_tree* t, RBT_node* x) {
    RBT_node* w;
    while (x != t->root && x->color == RBT_BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            // case 1: x's sibling w is red
            if (w->color == RBT_RED) {
                w->color = RBT_BLACK;
                x->parent->color = RBT_RED;

                rbt_left_rotate(t, x->parent);
                w = x->parent->right;
            }
            // case 2: x's sibling w is black and both of w's children are black
            if (w->left->color == RBT_BLACK && w->right->color == RBT_BLACK) {
                w->color = RBT_RED;

                x = x->parent;
            }
            else {
                // case 3: x's sibling w is black, w's left child is red, and
                // w's right child is black
                if (w->right->color == RBT_BLACK) {
                    w->left->color = RBT_BLACK;
                    w->color = RBT_RED;

                    rbt_right_rotate(t, w);
                    w = x->parent->right;
                }
                // case 4: x's sibling w is black and w's right child is red
                w->color = x->parent->color;
                x->parent->color = RBT_BLACK;
                w->right->color = RBT_BLACK;

                rbt_left_rotate(t, x->parent);
                x = t->root;
            }
        }
        else {
        // symmetric to the upper code
            w = x->parent->left;

            if (w->color == RBT_RED) {
                w->color = RBT_BLACK;
                x->parent->color = RBT_RED;

                rbt_right_rotate(t, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == RBT_BLACK && w->left->color == RBT_BLACK) {
                w->color = RBT_RED;

                x = x->parent;
            }
            else {
                if (w->left->color == RBT_BLACK) {
                    w->right->color = RBT_BLACK;
                    w->color = RBT_RED;

                    rbt_left_rotate(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBT_BLACK;
                w->left->color = RBT_BLACK;

                rbt_right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBT_BLACK;
}

static void rbt_node_delete(RB_tree* t, RBT_node* z) {
    RBT_node* x, *y;

    y = z;
    char y_original_color = y->color;
    
    if (z->left == NIL) {
        x = z->right;
        rbt_transplant(t, z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        rbt_transplant(t, z, z->left);
    } else  {
        y = rbt_first_node(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbt_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbt_transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == RBT_BLACK) {
        rbt_delete_fixup(t, x);
    }

    node_destroy(z);
}

/*
    Delete the iterator from the rbt.
    the key in the iter must be freed before this function is called
    if it was malloc()ed.
    return the next iterator of the iter,
    NULL if the next iterator doesn't exist
*/
RBT_iterator* rbt_delete_iterator(RB_tree* rbt, RBT_iterator* iter) {
    // assert(rbt != NULL && iter != NULL);
    RBT_node* next_node = rbt_node_next((RBT_node*)iter);

    rbt_node_delete(rbt, (RBT_node*)iter);

    return (next_node == NIL) ? NULL : &next_node->it;
}

/*
    Delete all keys int rbt equivalent to the key.
    return the number of the deleted keys, 
*/
int rbt_delete(RB_tree* rbt, void* key) {
    // assert(rbt != NULL);
    int n_deleted_key = 0;
    RBT_node* first_node = rbt_find_first_node_with_key(rbt, key);
    if (first_node == NIL) {
        return 0;
    }

    RBT_iterator* fit = &first_node->it;
    RBT_iterator* lit = rbt_find_after(rbt, key);
    while (fit != lit) {
        fit = rbt_delete_iterator(rbt, fit);
        n_deleted_key++;
    }

    return n_deleted_key;
}

// /*
//     Delete the key from the rbt.
//     return the deleted key, 
//     NULL on error or if the key doesn't exist in the tree
// */
// void* rbt_delete(RB_tree* t, void* key) {
//     RBT_node* x, *y, *z;
//     // z = (RBT_node*)node;
//     z = rbt_find_node(t, key);
//     if (z == NIL) {
//         return NULL;
//     }

//     y = z;
//     char y_original_color = y->color;
    
//     if (z->left == NIL) {
//         x = z->right;
//         rbt_transplant(t, z, z->right);
//     } else if (z->right == NIL) {
//         x = z->left;
//         rbt_transplant(t, z, z->left);
//     } else  {
//         y = rbt_first_node(z->right);
//         y_original_color = y->color;
//         x = y->right;
//         if (y->parent == z) {
//             x->parent = y;
//         } else {
//             rbt_transplant(t, y, y->right);
//             y->right = z->right;
//             y->right->parent = y;
//         }
//         rbt_transplant(t, z, y);
//         y->left = z->left;
//         y->left->parent = y;
//         y->color = z->color;
//     }

//     if (y_original_color == RBT_BLACK) {
//         rbt_delete_fixup(t, x);
//     }

//     node_destroy(z);
//     return key;
// }

// /*
//     Get an iterator which point a node with the key
//     return the iterator,
//     NULL on error or if the given key doesn't exist in the tree.
// */
// RBT_iterator* rbt_get_iterator(RB_tree* rbt, void* key) {
//     RBT_node* node = rbt_find_node(rbt, key);
//     return (node == NIL) ? NULL : &node->it;
// }

// RBT_iterator* rbt_first_iterator(RB_tree* rbt) {
//     RBT_node* node = rbt_first_node(rbt->root);
//     return (node == NIL) ? NULL : &node->it;
// }

// RBT_iterator* rbt_last_iterator(RB_tree* rbt) {
//     RBT_node* node = rbt_last_node(rbt->root);
//     return (node == NIL) ? NULL : &node->it;
// }

// void* rbt_iterator_get_key(RBT_iterator* iter) {
//     return (iter == NULL)? NULL : iter->key;
// }





// void inorder_traverse(RBT_node* root) {
//     if (root->left != NIL) {
//         inorder_traverse(root->left);
//     }
//     printf("%ld ", (__intptr_t)root->key);

//     if (root->right != NIL) {
//         inorder_traverse(root->right);
//     }
// }

// void rbt_inorder_traverse(RB_tree* rbt) {
//     printf("tree: ");
//     if (rbt->root != NULL)
//         inorder_traverse(rbt->root);
//     printf("\n");
// }

// void* rbt_find_next(RB_tree* rbt, void* key) {
//     RBT_node* x = rbt_find_node(rbt, key);
//     if (x == NIL) {
//         return NULL;
//     }

//     if (x->right != NIL) {
//         // RBT_node* first = rbt_first_node(x->right);
//         // return first->key;
//         return rbt_first_node(x->right)->it.key;
//     }
//     RBT_node* y = x->parent;
//     while (y != NIL && x == y->right) {
//         x = y;
//         y = y->parent;
//     }

//     // return (y == NIL) ? NULL : y->key
//     return y->it.key;
// }

// void* rbt_find_prev(RB_tree* rbt, void* key) {
//     RBT_node* x = rbt_find_node(rbt, key);
//     if (x == NIL) {
//         return NULL;
//     }

//     if (x->left != NIL) {
//         // RBT_node* last = rbt_last_node(x->left);
//         // return last->key;
//         return rbt_last_node(x->left)->it.key;
//     }
//     RBT_node* y = x->parent;
//     while (y != NIL && x == y->left) {
//         x = y;
//         y = y->parent;
//     }

//     // return (y == NIL) ? NULL : y->key
//     return y->it.key;
// }