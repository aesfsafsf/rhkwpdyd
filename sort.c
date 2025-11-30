#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// 데이터 최대 크기 (CSV가 몇 만 줄인지 모르므로 여유 있게)
#define MAX 200000
#define REPEAT 1000

// -----------------------------
// 구조체 정의
// -----------------------------
typedef struct {
    int id;
    char name[100];
    char gender;
    int kor, eng, math;
    int grade_sum;
} Student;

// -----------------------------
long long cmp_count;

#define CMP(a, b) (cmp_count++, ((a) > (b)) - ((a) < (b)))

typedef enum {
    SORT_ID_ASC, SORT_ID_DESC,
    SORT_NAME_ASC, SORT_NAME_DESC,
    SORT_GENDER_ASC, SORT_GENDER_DESC,
    SORT_GRADE_ASC, SORT_GRADE_DESC
} SortType;

// 비교 함수
int compare(Student a, Student b, SortType type) {
    int t;
    switch (type) {
    case SORT_ID_ASC:  return CMP(a.id, b.id);
    case SORT_ID_DESC: return CMP(b.id, a.id);

    case SORT_NAME_ASC:  return CMP(strcmp(a.name, b.name), 0);
    case SORT_NAME_DESC: return CMP(strcmp(b.name, a.name), 0);

    case SORT_GENDER_ASC:  return CMP(a.gender, b.gender);
    case SORT_GENDER_DESC: return CMP(b.gender, a.gender);

    case SORT_GRADE_ASC:
    case SORT_GRADE_DESC: {
        int sign = (type == SORT_GRADE_ASC ? 1 : -1);

        if ((t = CMP(a.grade_sum, b.grade_sum)) != 0) return sign * t;
        if ((t = CMP(a.kor, b.kor)) != 0) return -t;
        if ((t = CMP(a.eng, b.eng)) != 0) return -t;
        return -CMP(a.math, b.math);
    }
    }
    return 0;
}

void swap(Student* a, Student* b) {
    Student t = *a;
    *a = *b;
    *b = t;
}

// -----------------------------
// 정렬 알고리즘 (9개)
// -----------------------------
void bubble_sort(Student arr[], int n, SortType type) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (compare(arr[j], arr[j + 1], type) > 0)
                swap(&arr[j], &arr[j + 1]);
}

void selection_sort(Student arr[], int n, SortType type) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (compare(arr[j], arr[min], type) < 0)
                min = j;
        swap(&arr[i], &arr[min]);
    }
}

void insertion_sort(Student arr[], int n, SortType type) {
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0 && compare(arr[j], key, type) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void shell_sort(Student arr[], int n, SortType type) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            for (j = i; j >= gap && compare(arr[j - gap], temp, type) > 0; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

// Quick Sort
int partition(Student arr[], int low, int high, SortType type) {
    Student pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
        if (compare(arr[j], pivot, type) <= 0)
            swap(&arr[++i], &arr[j]);

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort(Student arr[], int low, int high, SortType type) {
    if (low < high) {
        int pi = partition(arr, low, high, type);
        quick_sort(arr, low, pi - 1, type);
        quick_sort(arr, pi + 1, high, type);
    }
}

// Heap Sort
void heapify(Student arr[], int n, int i, SortType type) {
    int largest = i, L = 2 * i + 1, R = 2 * i + 2;

    if (L < n && compare(arr[L], arr[largest], type) > 0)
        largest = L;
    if (R < n && compare(arr[R], arr[largest], type) > 0)
        largest = R;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest, type);
    }
}

void heap_sort(Student arr[], int n, SortType type) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, type);

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0, type);
    }
}

// Merge Sort
void merge(Student arr[], int l, int m, int r, SortType type) {
    int n1 = m - l + 1, n2 = r - m;

    Student* L = malloc(sizeof(Student) * n1);
    Student* R = malloc(sizeof(Student) * n2);

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
        arr[k++] = (compare(L[i], R[j], type) <= 0) ? L[i++] : R[j++];

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_sort(Student arr[], int l, int r, SortType type) {
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort(arr, l, m, type);
        merge_sort(arr, m + 1, r, type);
        merge(arr, l, m, r, type);
    }
}

// Radix Sort (ID only)
int getMax(Student arr[], int n) {
    int mx = arr[0].id;
    for (int i = 1; i < n; i++)
        if (arr[i].id > mx) mx = arr[i].id;
    return mx;
}

void countSort(Student arr[], int n, int exp) {
    Student* output = malloc(sizeof(Student) * n);
    int count[10] = { 0 };

    for (int i = 0; i < n; i++)
        count[(arr[i].id / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i].id / exp) % 10] - 1] = arr[i];
        count[(arr[i].id / exp) % 10]--;
    }

    memcpy(arr, output, sizeof(Student) * n);
    free(output);
}

void radix_sort(Student arr[], int n) {
    int m = getMax(arr, n);
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

// Tree Sort
typedef struct Node {
    Student data;
    struct Node* left, * right;
} Node;

Node* insert_node(Node* root, Student data, SortType type) {
    if (!root) {
        Node* node = malloc(sizeof(Node));
        node->data = data;
        node->left = node->right = NULL;
        return node;
    }
    if (compare(data, root->data, type) < 0)
        root->left = insert_node(root->left, data, type);
    else
        root->right = insert_node(root->right, data, type);

    return root;
}

void inorder(Node* root, Student arr[], int* i) {
    if (!root) return;
    inorder(root->left, arr, i);
    arr[(*i)++] = root->data;
    inorder(root->right, arr, i);
}

void tree_sort(Student arr[], int n, SortType type) {
    Node* root = NULL;

    for (int i = 0; i < n; i++)
        root = insert_node(root, arr[i], type);

    int idx = 0;
    inorder(root, arr, &idx);
}

// -----------------------------
// 파일 로드
// -----------------------------
int load_data(Student arr[]) {
    FILE* fp = fopen("C:/Users/USER/Desktop/dataset_id_ascending.csv", "r");
    if (!fp) {
        printf("파일 열기 실패\n");
        return 0;
    }

    printf("파일 열기 성공!\n");

    char line[300];
    fgets(line, sizeof(line), fp); // 첫 줄 제거

    int count = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (count >= MAX) {
            printf("경고: MAX 초과! 더 이상 로드하지 않습니다.\n");
            break;
        }

        if (sscanf(line, "%d,%99[^,],%c,%d,%d,%d",
            &arr[count].id,
            arr[count].name,
            &arr[count].gender,
            &arr[count].kor,
            &arr[count].eng,
            &arr[count].math) == 6)
        {
            arr[count].grade_sum =
                arr[count].kor + arr[count].eng + arr[count].math;
            count++;
        }
    }

    fclose(fp);
    printf("로드된 데이터: %d개\n", count);
    return count;
}

// -----------------------------
// 실행 함수
// -----------------------------
void run_sort(void (*sort_func)(Student*, int, SortType),
    Student base[], int n, SortType type,
    const char* name)
{
    long long total = 0;

    Student* temp = malloc(sizeof(Student) * n);

    for (int r = 0; r < REPEAT; r++) {
        memcpy(temp, base, sizeof(Student) * n);
        cmp_count = 0;

        sort_func(temp, n, type);
        total += cmp_count;
    }

    printf("[%s] 평균 비교 횟수: %lld\n", name, total / REPEAT);
    free(temp);
}

// -----------------------------
// 메인 함수
// -----------------------------
int main() {
    Student* arr = malloc(sizeof(Student) * MAX);

    int n = load_data(arr);
    printf("데이터 %d개 로드 완료\n", n);

    run_sort(bubble_sort, arr, n, SORT_ID_ASC, "버블 정렬");
    run_sort(selection_sort, arr, n, SORT_ID_ASC, "선택 정렬");
    run_sort(insertion_sort, arr, n, SORT_ID_ASC, "삽입 정렬");
    run_sort(shell_sort, arr, n, SORT_ID_ASC, "셸 정렬");

    // Quick Sort
    for (int r = 0; r < REPEAT; r++) {
        Student* tmp = malloc(sizeof(Student) * n);
        memcpy(tmp, arr, sizeof(Student) * n);
        cmp_count = 0;
        quick_sort(tmp, 0, n - 1, SORT_ID_ASC);
        free(tmp);
    }
    printf("퀵 정렬 완료\n");

    run_sort(heap_sort, arr, n, SORT_ID_ASC, "힙 정렬");

    // Merge Sort
    for (int r = 0; r < REPEAT; r++) {
        Student* tmp = malloc(sizeof(Student) * n);
        memcpy(tmp, arr, sizeof(Student) * n);
        cmp_count = 0;
        merge_sort(tmp, 0, n - 1, SORT_ID_ASC);
        free(tmp);
    }
    printf("병합 정렬 완료\n");

    // Radix Sort
    for (int r = 0; r < REPEAT; r++) {
        Student* tmp = malloc(sizeof(Student) * n);
        memcpy(tmp, arr, sizeof(Student) * n);
        radix_sort(tmp, n);
        free(tmp);
    }
    printf("기수 정렬 완료\n");

    // Tree Sort
    for (int r = 0; r < REPEAT; r++) {
        Student* tmp = malloc(sizeof(Student) * n);
        memcpy(tmp, arr, sizeof(Student) * n);
        cmp_count = 0;
        tree_sort(tmp, n, SORT_ID_ASC);
        free(tmp);
    }
    printf("트리 정렬 완료\n");

    free(arr);
    return 0;
}
