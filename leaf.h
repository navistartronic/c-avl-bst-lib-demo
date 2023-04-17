/* User defined standard data structure to store in the tree as a tree leaf */
#define LEAF_KEYLEN 30

typedef
    struct leafnode {
    char key[LEAF_KEYLEN + 1];
    int data;
} Leaf;
