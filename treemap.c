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
    if (tree == NULL || key == NULL || value == NULL) {
        return;
    }

    TreeNode* newNode = createTreeNode(key, value);
    if (newNode == NULL) {
        return;
    }

    if (tree->root == NULL) {
        tree->root = newNode;
        tree->current = newNode;
        return;
    }

    TreeNode* current = tree->root;
    TreeNode* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) { 
            current = current->right;
        } else {
            free(newNode->pair);
            free(newNode);
            return;
        }
    }

    if (tree->lower_than(key, parent->pair->key)) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    newNode->parent = parent;
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
    TreeNode* child = NULL;

    if (node->left == NULL || node->right == NULL) {
        if (node->left == NULL && node->right == NULL) {
            child = NULL;
        } else if (node->left != NULL) {
            child = node->left;
        } else {
            child = node->right;
        }

        if (parent != NULL) {
            if (parent->left == node) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            if (child != NULL) {
                child->parent = parent;
            }
        } else {
            tree->root = child;
            if (child != NULL) {
                child->parent = NULL;
            }
        }
        free(node->pair);
        free(node);
    } else {
        TreeNode* minRight = minimum(node->right);
        node->pair->key = minRight->pair->key;
        node->pair->value = minRight->pair->value;
        removeNode(tree, minRight);
    }
}



void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair* searchTreeMap(TreeMap* tree, void* key) {
    TreeNode* currentNode = tree->root;

    while (currentNode != NULL) {
        int equal = is_equal(tree, key, currentNode->pair->key);

        if (equal) {
            tree->current = currentNode;
            return currentNode->pair;
        } else if (tree->lower_than(key, currentNode->pair->key) > 0) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }
    tree->current = NULL;
    return NULL;
}

Pair* upperBound(TreeMap* tree, void* key) {
    if (tree == NULL || tree->root == NULL) {
        return NULL;
    }

    TreeNode* current = tree->root;
    Pair* ub_pair = NULL;

    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key)) {
            if (ub_pair == NULL || tree->lower_than(current->pair->key, ub_pair->key)) {
                ub_pair = current->pair;
            }
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else {
            Pair* result = (Pair*)malloc(sizeof(Pair));
            if (result != NULL) {
                result->key = current->pair->key;
                result->value = current->pair->value;
            }
            return result;
        }
    }

    if (ub_pair != NULL) {
        Pair* result = (Pair*)malloc(sizeof(Pair));
        if (result != NULL) {
            result->key = ub_pair->key;
            result->value = ub_pair->value;
        }
        return result;
    }

    return NULL;
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
