#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 1000

typedef struct BSTNode {
    int key;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct AVLNode {
    int key;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

BSTNode* insertBST(BSTNode* root, int key) {
    if (root == NULL) {
        BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
        newNode->key = key;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (key < root->key) {
        root->left = insertBST(root->left, key);
    }
    else if (key > root->key) {
        root->right = insertBST(root->right, key);
    }
    return root;
}

int searchBST(BSTNode* root, int key, int count) {
    if (root == NULL) return count;
    count++;
    if (key < root->key) return searchBST(root->left, key, count);
    if (key > root->key) return searchBST(root->right, key, count);
    return count;
}

int getHeight(AVLNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

int getBalance(AVLNode* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + ((getHeight(y->left) > getHeight(y->right)) ? getHeight(y->left) : getHeight(y->right));
    x->height = 1 + ((getHeight(x->left) > getHeight(x->right)) ? getHeight(x->left) : getHeight(x->right));
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + ((getHeight(x->left) > getHeight(x->right)) ? getHeight(x->left) : getHeight(x->right));
    y->height = 1 + ((getHeight(y->left) > getHeight(y->right)) ? getHeight(y->left) : getHeight(y->right));
    return y;
}

AVLNode* insertAVL(AVLNode* node, int key) {
    if (node == NULL) {
        AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
        newNode->key = key;
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }
    if (key < node->key) {
        node->left = insertAVL(node->left, key);
    }
    else {
        node->right = insertAVL(node->right, key);
    }

    node->height = 1 + ((getHeight(node->left) > getHeight(node->right)) ? getHeight(node->left) : getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key) return rightRotate(node);

    if (balance < -1 && key > node->right->key) return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

int searchAVL(AVLNode* root, int key, int count) {
    if (root == NULL) return count;
    count++;
    if (key < root->key) return searchAVL(root->left, key, count);
    if (key > root->key) return searchAVL(root->right, key, count);
    return count;
}

int linearSearch(int arr[], int size, int key) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        count++;
        if (arr[i] == key) return count;
    }
    return count;
}

void generateRandomNumbers(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 10001;
    }
}

double calculateAverage(int counts[], int size) {
    int total = 0;
    for (int i = 0; i < size; i++) {
        total += counts[i];
    }
    return (double)total / size;
}

int main() {
    srand(time(NULL));

    int data1[MAX_SIZE];
    int data2[MAX_SIZE];
    int data3[MAX_SIZE];
    int data4[MAX_SIZE];

    generateRandomNumbers(data1, MAX_SIZE);

    for (int i = 0; i < MAX_SIZE; i++) {
        data2[i] = i;
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        data3[i] = MAX_SIZE - 1 - i;
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        data4[i] = i * (i % 2 + 2);
    }

    BSTNode* bstRoot = NULL;
    AVLNode* avlRoot = NULL;

    for (int i = 0; i < MAX_SIZE; i++) {
        bstRoot = insertBST(bstRoot, data2[i]);
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        avlRoot = insertAVL(avlRoot, data3[i]);
    }

    int searchNumbers[MAX_SIZE];
    generateRandomNumbers(searchNumbers, MAX_SIZE);

    int arraySearchCounts[MAX_SIZE];
    int bstSearchCounts[MAX_SIZE];
    int avlSearchCounts[MAX_SIZE];

    for (int i = 0; i < MAX_SIZE; i++) {
        arraySearchCounts[i] = linearSearch(data1, MAX_SIZE, searchNumbers[i]);
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        bstSearchCounts[i] = searchBST(bstRoot, searchNumbers[i], 0);
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        avlSearchCounts[i] = searchAVL(avlRoot, searchNumbers[i], 0);
    }

    printf("Array Æò±Õ Å½»ö È½¼ö: %.2fÈ¸\n", calculateAverage(arraySearchCounts, MAX_SIZE));
    printf("BST Æò±Õ Å½»ö È½¼ö: %.2fÈ¸\n", calculateAverage(bstSearchCounts, MAX_SIZE));
    printf("AVL Æò±Õ Å½»ö È½¼ö: %.2fÈ¸\n", calculateAverage(avlSearchCounts, MAX_SIZE));

    return 0;
}