/*
 * Q11: BST for government database ID numbers
 * Keys are compared as STRINGS (lexicographic order), since IDs like
 * "A102" are alphanumeric codes, not pure integers.
 *
 * Compile:  gcc bst_lab.c -o bst_lab
 * Run:      ./bst_lab
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY 10

typedef struct Node {
    char key[MAX_KEY];
    struct Node *left, *right;
} Node;

/* ---------- BST core ---------- */

Node* newNode(const char *k) {
    Node *n = malloc(sizeof(Node));
    strcpy(n->key, k);
    n->left = n->right = NULL;
    return n;
}

Node* insert(Node *root, const char *k) {
    if (root == NULL) return newNode(k);
    if (strcmp(k, root->key) < 0)
        root->left = insert(root->left, k);
    else if (strcmp(k, root->key) > 0)
        root->right = insert(root->right, k);
    return root; /* duplicates ignored */
}

void inorder(Node *root) {
    if (!root) return;
    inorder(root->left);
    printf("%s ", root->key);
    inorder(root->right);
}

int height(Node *root) {
    if (!root) return -1; /* empty tree height = -1, single node = 0 */
    int lh = height(root->left);
    int rh = height(root->right);
    return 1 + (lh > rh ? lh : rh);
}

int nodeCount(Node *root) {
    if (!root) return 0;
    return 1 + nodeCount(root->left) + nodeCount(root->right);
}

/* ---------- Search with comparison counting ---------- */

int bstSearch(Node *root, const char *k, int *comparisons) {
    Node *cur = root;
    while (cur) {
        (*comparisons)++;
        int c = strcmp(k, cur->key);
        if (c == 0) return 1;
        cur = (c < 0) ? cur->left : cur->right;
    }
    return 0;
}

int linearSearch(char keys[][MAX_KEY], int n, const char *target, int *comparisons) {
    for (int i = 0; i < n; i++) {
        (*comparisons)++;
        if (strcmp(keys[i], target) == 0) return 1;
    }
    return 0;
}

/* ---------- Driver ---------- */

int main() {
    char ids[][MAX_KEY] = {"A102", "A25", "A7", "B100", "B12", "A120", "B3", "A45"};
    int n = sizeof(ids) / sizeof(ids[0]);

    Node *root = NULL;
    for (int i = 0; i < n; i++)
        root = insert(root, ids[i]);

    /* a) Inorder traversal + structure analysis */
    printf("a) Inorder traversal (sorted ID order):\n   ");
    inorder(root);
    printf("\n");
    printf("   Height of BST      : %d\n", height(root));
    printf("   Number of nodes    : %d\n", nodeCount(root));
    printf("   (Perfectly balanced height for 8 nodes would be 3)\n\n");

    /* b) BST search vs Linear search, comparison counts */
    char *queries[] = {"A102", "B3", "A45", "B999"}; /* last one = not found */
    int nq = sizeof(queries) / sizeof(queries[0]);

    printf("b) BST Search vs Linear Search (comparison counts):\n");
    printf("   %-8s %-10s %-14s\n", "Key", "BST comps", "Linear comps");
    for (int i = 0; i < nq; i++) {
        int bstC = 0, linC = 0;
        int foundB = bstSearch(root, queries[i], &bstC);
        int foundL = linearSearch(ids, n, queries[i], &linC);
        printf("   %-8s %-10d %-14d %s\n", queries[i], bstC, linC,
               foundB ? "(found)" : "(not found)");
        (void)foundL;
    }

    return 0;
}
