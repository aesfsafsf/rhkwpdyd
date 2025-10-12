#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*이 코드에서는 탐색 시간이 동일했지만, 시행 횟수를 늘렸을 때는 이진탐색트리의 검색 시간이 더 짧았음.*/

typedef struct TreeNode {
    int key;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* insertNode(TreeNode* root, int key);
TreeNode* search(TreeNode* root, int key);
void freeTree(TreeNode* root);

int main(void) {
    int arr[100];
    TreeNode* root = NULL;
    int i, target, found = 0;
    clock_t start, end;
    double linear_time, tree_time;

    srand(time(NULL));

    printf("배열 원소들:\n");
    for (i = 0; i < 100; i++) {
        arr[i] = rand() % 1001;
        printf("%4d ", arr[i]);
        if ((i + 1) % 10 == 0) printf("\n");
        root = insertNode(root, arr[i]);
    }

    target = arr[rand() % 100];
    printf("\n탐색할 숫자: %d\n", target);

    start = clock();
    for (i = 0; i < 100; i++) {
        if (arr[i] == target) {
            found = 1;
            break;
        }
    }
    end = clock();
    linear_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    if (found)
        printf("배열에서 숫자 %d 를 인덱스 %d 에서 찾았습니다.\n", target, i);
    else
        printf("배열에서 숫자 %d 를 찾지 못했습니다.\n", target);

    start = clock();
    TreeNode* result = search(root, target);
    end = clock();
    tree_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    if (result != NULL)
        printf("이진탐색트리에서 숫자 %d 를 찾았습니다.\n", target);
    else
        printf("이진탐색트리에서 숫자 %d 를 찾지 못했습니다.\n", target);

    printf("\n===== 탐색 속도 비교 =====\n");
    printf("배열 선형 탐색 시간: %.6f ms\n", linear_time);
    printf("이진탐색트리 탐색 시간: %.6f ms\n", tree_time);

    freeTree(root);

    return 0;
}

TreeNode* insertNode(TreeNode* root, int key) {
    if (root == NULL) {
        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
        newNode->key = key;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (key < root->key)
        root->left = insertNode(root->left, key);
    else if (key > root->key)
        root->right = insertNode(root->right, key);
    return root;
}

TreeNode* search(TreeNode* root, int key) {
    if (root == NULL) return NULL;
    if (key == root->key) return root;
    else if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}

void freeTree(TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}