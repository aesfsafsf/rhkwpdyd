#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 200

char tree[MAX];

void buildTree(char* expr) {
    int stack[MAX], top = -1;
    int parent = 0;
    int isRight = 0;
    int idx = 0;

    for (int i = 0; expr[i]; i++) {
        char c = expr[i];
        if (isalpha(c)) {
            if (parent == 0) {
                idx = 1;
            }
            else if (isRight == 0) {
                idx = parent * 2;
            }
            else {
                idx = parent * 2 + 1;
            }
            tree[idx] = c;
        }
        else if (c == '(') {
            stack[++top] = idx;
            parent = idx;
            isRight = 0;
        }
        else if (c == ')') {
            parent = stack[top--];
        }
        else if (c == ' ') {
            isRight = 1;
        }
    }
}

void preorder() {
    int stack[MAX], top = -1;
    stack[++top] = 1;
    while (top >= 0) {
        int cur = stack[top--];
        if (cur >= MAX || tree[cur] == 0) continue;
        printf("%c ", tree[cur]);
        stack[++top] = cur * 2 + 1;
        stack[++top] = cur * 2;
    }
    printf("\n");
}

void inorder() {
    int stack[MAX], top = -1;
    int cur = 1;
    while (cur < MAX || top >= 0) {
        if (cur < MAX && tree[cur] != 0) {
            stack[++top] = cur;
            cur = cur * 2;
        }
        else {
            cur = stack[top--];
            printf("%c ", tree[cur]);
            cur = cur * 2 + 1;
        }
    }
    printf("\n");
}

void postorder() {
    int stack[MAX];
    int visited[MAX] = { 0 };
    int top = -1;
    int cur = 1;

    while (cur < MAX || top >= 0) {
        if (cur < MAX && tree[cur] != 0 && !visited[cur]) {
            stack[++top] = cur;
            cur = cur * 2;
        }
        else {
            cur = stack[top];
            if (!visited[cur]) {
                visited[cur] = 1;
                cur = cur * 2 + 1;
            }
            else {
                printf("%c ", tree[cur]);
                top--;
                cur = MAX;
            }
        }
    }
    printf("\n");
}

int main() {
    char input[300];
    fgets(input, sizeof(input), stdin);

    memset(tree, 0, sizeof(tree));
    buildTree(input);

    printf("pre-order: ");
    preorder();
    printf("in-order: ");
    inorder();
    printf("post-order: ");
    postorder();
    return 0;
}
