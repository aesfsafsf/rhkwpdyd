#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200

// -------------------- Student 구조체 --------------------
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
} Student;

// -------------------- 학생 데이터 로드 --------------------
Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 10;
    int count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);

    if (!arr) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    fgets(line, sizeof(line), fp);  // header skip

    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* temp = realloc(arr, sizeof(Student) * capacity);
            if (!temp) {
                perror("Reallocation failed");
                free(arr);
                fclose(fp);
                return NULL;
            }
            arr = temp;
        }

        Student s;
        char* token = strtok(line, ",");
        s.id = atoi(token);

        token = strtok(NULL, ",");
        strncpy(s.name, token, MAX_NAME_LEN);

        token = strtok(NULL, ",");
        s.gender = token[0];

        token = strtok(NULL, ","); s.korean = atoi(token);
        token = strtok(NULL, ","); s.english = atoi(token);
        token = strtok(NULL, ","); s.math = atoi(token);

        arr[count++] = s;
    }

    fclose(fp);

    Student* tight = realloc(arr, sizeof(Student) * count);
    if (!tight) {
        fprintf(stderr, "Warning: Tight reallocation failed.\n");
        *out_count = count;
        return arr;
    }

    *out_count = count;
    return tight;
}

// =============================================================
//                   [A] 비정렬 배열
// =============================================================
int unsorted_search(Student arr[], int n, int key, int* comp) {
    for (int i = 0; i < n; i++) {
        (*comp)++;
        if (arr[i].id == key) return i;
    }
    return -1;
}

void unsorted_insert(Student arr[], int* n, Student s) {
    arr[(*n)++] = s;  // 비교 없음
}

int unsorted_delete(Student arr[], int* n, int key, int* comp) {
    int idx = unsorted_search(arr, *n, key, comp);
    if (idx == -1) return 0;

    arr[idx] = arr[*n - 1];
    (*n)--;
    return 1;
}

// =============================================================
//                   [B] 정렬 배열
// =============================================================
int compare_student(const void* a, const void* b) {
    return ((Student*)a)->id - ((Student*)b)->id;
}

int sorted_binary_search(Student arr[], int n, int key, int* comp) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        (*comp)++;
        if (arr[mid].id == key) return mid;
        else if (arr[mid].id < key) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void sorted_insert(Student arr[], int* n, Student s, int* comp) {
    int left = 0, right = *n - 1, pos = *n;

    while (left <= right) {
        int mid = (left + right) / 2;
        (*comp)++;
        if (arr[mid].id < s.id) left = mid + 1;
        else { pos = mid; right = mid - 1; }
    }

    for (int i = *n; i > pos; i--)
        arr[i] = arr[i - 1];

    arr[pos] = s;
    (*n)++;
}

int sorted_delete(Student arr[], int* n, int key, int* comp) {
    int idx = sorted_binary_search(arr, *n, key, comp);
    if (idx == -1) return 0;

    for (int i = idx; i < *n - 1; i++)
        arr[i] = arr[i + 1];

    (*n)--;
    return 1;
}

// =============================================================
//                   [C] AVL 트리
// =============================================================
#define CMP(x, y, comp) ((*comp)++, ((x) - (y)))

typedef struct AVLNode {
    Student data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

int height(AVLNode* n) {
    return n ? n->height : 0;
}

int getBalance(AVLNode* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

AVLNode* newNode(Student s) {
    AVLNode* n = malloc(sizeof(AVLNode));
    n->data = s;
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

// ---- 회전 ----
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    return y;
}

AVLNode* avl_balance(AVLNode* node) {
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);

    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);

    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// ---- 삽입 ----
AVLNode* avl_insert(AVLNode* node, Student s, int* comp) {
    if (!node) return newNode(s);

    int cmp = CMP(s.id, node->data.id, comp);

    if (cmp < 0)
        node->left = avl_insert(node->left, s, comp);
    else if (cmp > 0)
        node->right = avl_insert(node->right, s, comp);
    else
        return node;

    node->height = 1 + (height(node->left) > height(node->right) ?
        height(node->left) : height(node->right));

    return avl_balance(node);
}

// ---- 검색 ----
AVLNode* avl_search(AVLNode* root, int key, int* comp) {
    if (!root) return NULL;

    int cmp = CMP(key, root->data.id, comp);

    if (cmp == 0) return root;
    else if (cmp < 0) return avl_search(root->left, key, comp);
    else return avl_search(root->right, key, comp);
}

// ---- 삭제 ----
AVLNode* avl_delete(AVLNode* root, int key, int* comp) {
    if (!root) return NULL;

    int cmp = CMP(key, root->data.id, comp);

    if (cmp < 0)
        root->left = avl_delete(root->left, key, comp);
    else if (cmp > 0)
        root->right = avl_delete(root->right, key, comp);
    else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;

            if (!temp) {
                free(root);
                return NULL;
            }
            *root = *temp;
            free(temp);
        }
        else {
            AVLNode* temp = root->right;
            while (temp->left) temp = temp->left;

            root->data = temp->data;
            root->right = avl_delete(root->right, temp->data.id, comp);
        }
    }

    root->height = 1 + (height(root->left) > height(root->right) ?
        height(root->left) : height(root->right));

    return avl_balance(root);
}

// =============================================================
//                        MAIN
// =============================================================
int main() {
    int count;
    Student* students = load_students("students.csv", &count);

    if (!students) return 1;

    printf("Loaded %d students.\n\n", count);

    // -----------------------------
    // A. 비정렬 배열 테스트
    // -----------------------------
    Student* unsorted = malloc(sizeof(Student) * (count + 100));
    memcpy(unsorted, students, sizeof(Student) * count);
    int u_count = count;

    int comp;

    comp = 0;
    unsorted_search(unsorted, u_count, students[10].id, &comp);
    printf("[Unsorted] Search comparisons = %d\n", comp);

    comp = 0;
    unsorted_delete(unsorted, &u_count, students[20].id, &comp);
    printf("[Unsorted] Delete comparisons = %d\n", comp);

    // -----------------------------
    // B. 정렬 배열 테스트
    // -----------------------------
    Student* sorted = malloc(sizeof(Student) * (count + 100));
    memcpy(sorted, students, sizeof(Student) * count);
    int s_count = count;

    qsort(sorted, s_count, sizeof(Student), compare_student);

    comp = 0;
    sorted_binary_search(sorted, s_count, students[10].id, &comp);
    printf("[Sorted] Binary Search comparisons = %d\n", comp);

    comp = 0;
    sorted_delete(sorted, &s_count, students[20].id, &comp);
    printf("[Sorted] Delete comparisons = %d\n", comp);

    // -----------------------------
    // C. AVL 트리 테스트
    // -----------------------------
    AVLNode* root = NULL;

    for (int i = 0; i < count; i++) {
        comp = 0;
        root = avl_insert(root, students[i], &comp);
        // printf("AVL Insert %d → comparisons = %d\n", students[i].id, comp);
    }

    comp = 0;
    avl_search(root, students[10].id, &comp);
    printf("[AVL] Search comparisons = %d\n", comp);

    comp = 0;
    root = avl_delete(root, students[20].id, &comp);
    printf("[AVL] Delete comparisons = %d\n", comp);

    free(unsorted);
    free(sorted);
    free(students);

    printf("\nAll tests completed.\n");
    return 0;
}
