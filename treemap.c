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



void removeNode(TreeMap * tree, TreeNode* node) {

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




Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
