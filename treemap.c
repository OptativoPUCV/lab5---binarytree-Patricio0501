#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * map = (TreeMap *)malloc(sizeof(TreeMap));
    if (map == NULL){
      return NULL;
    }
  
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;

    return map;
}

void insertTreeMap(TreeMap* tree, void* key, void* value) {
    if (tree == NULL || key == NULL) {
        return; 
    }
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;
    int cmp = 0;

    while (current != NULL) {
        cmp = tree->lower_than(key, current->pair->key);

        if (cmp == 0) {
            return;
        }

        parent = current;

        if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    TreeNode* newNode = createTreeNode(key, value);
    if (newNode == NULL) {
        return; 
    }
    newNode->parent = parent;

    if (parent == NULL) {
        tree->root = newNode;
    } else if (cmp < 0) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    tree->current = newNode;
}

TreeNode* minimum(TreeNode* x) {
    if (x == NULL) {
        return NULL; 
    }

    while (x->left != NULL) {
        x = x->left;
    }

    return x; 
}

void removeNode(TreeMap* tree, TreeNode* node) {
    if (tree == NULL || node == NULL) {
        return; 
    }

    TreeNode* parent = node->parent;

    if (node->left == NULL && node->right == NULL) {
        if (parent != NULL) {
            if (parent->left == node) {
                free(node);
            } else {
                free(node);
            }
        } else {
            tree->root = NULL;
        } 
        free(parent);
    }
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        if (parent != NULL) {
            if (parent->left == node) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            child->parent = parent;
        } else {
            tree->root = child;
            child->parent = NULL;
        }
        free(node->pair); 
        free(node);
    }
    else {
        TreeNode* minimumNode = minimum(node->right); 
        Pair* tempPair = node->pair;

        node->pair = minimumNode->pair;
        minimumNode->pair = tempPair;

        if (minimumNode->parent->left == minimumNode) {
            minimumNode->parent->left = minimumNode->right;
        } else {
            minimumNode->parent->right = minimumNode->right;
        }

        if (minimumNode->right != NULL) {
            minimumNode->right->parent = minimumNode->parent;
        }

        free(minimumNode->pair); 
        free(minimumNode);
    }
}



void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair* searchTreeMap(TreeMap* tree, void* key) {
    if (tree == NULL || key == NULL || tree->root == NULL) {
        return NULL; 
    }

    TreeNode* current = tree->root;

    while (current != NULL) {
        int cmp = tree->lower_than(key, current->pair->key);

        if (cmp == 0) {
            tree->current = current;
            return current->pair;
        }
        if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    tree->current = NULL;
    return NULL;
}

Pair* upperBound(TreeMap* tree, void* key) {
    if (tree == NULL || tree->root == NULL) {
        return NULL; // Árbol vacío o nulo
    }

    TreeNode* current = tree->root;
    TreeNode* ub_node = NULL; // Nodo con la menor clave mayor o igual a key

    while (current != NULL) {
        int cmp = tree->lower_than(key, current->pair->key);

        if (cmp == 0) {
            return current->pair; // Clave encontrada
        } else if (cmp < 0) {
            // Actualizar el nodo con la menor clave mayor o igual a key
            ub_node = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (ub_node != NULL) {
        return ub_node->pair;
    } else {
        return NULL; // No se encontró ningún nodo con clave mayor o igual a key
    }
}


Pair* firstTreeMap(TreeMap* tree) {
    if (tree == NULL || tree->root == NULL) {
        return NULL; 
    }
    TreeNode* minNode = minimum(tree->root);
    tree->current = minNode;

    if (minNode != NULL) {
        return minNode->pair;
    } else {
        return NULL;
    }
}
Pair* nextTreeMap(TreeMap* tree) {
    if (tree == NULL || tree->current == NULL) {
        return NULL; 
    }
    if (tree->current->right != NULL) {
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }
    TreeNode* parent = tree->current->parent;
    while (parent != NULL && tree->current == parent->right) {
        tree->current = parent;
        parent = parent->parent;
    }
    tree->current = parent;

    if (parent != NULL) {
        return parent->pair;
    } else {
        return NULL;
    }
}