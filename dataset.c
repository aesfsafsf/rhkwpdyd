#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 100   // 파일에도 100명이 존재함

typedef struct {
    int score1, score2, score3;
    long long product;
} Student;

long long bubble_sort(Student arr[], int n) {
    long long compare_count = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            compare_count++;
            if (arr[j].product > arr[j + 1].product) {
                Student temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return compare_count;
}

long long sequential_search(Student arr[], int n, long long target) {
    long long compare_count = 0;
    for (int i = 0; i < n; i++) {
        compare_count++;
        if (arr[i].product == target) break;
    }
    return compare_count;
}

long long binary_search(Student arr[], int n, long long target) {
    long long compare_count = 0;
    int left = 0, right = n - 1;

    while (left <= right) {
        compare_count++;
        int mid = (left + right) / 2;

        if (arr[mid].product == target) break;
        else if (arr[mid].product < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return compare_count;
}

int main() {
    Student students[N];
    Student sorted[N];

    FILE* fp = fopen("C:/Users/USER/Desktop/dataset_id_ascending.csv", "r");
    if (fp == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    char line[256];

    // 1. 헤더 제거
    fgets(line, sizeof(line), fp);

    // 2. 파일에서 점수 읽기
    for (int i = 0; i < N; i++) {
        int id;
        char name[50], gender[5];
        int k, e, m;

        fgets(line, sizeof(line), fp);

        // CSV 형식:
        // ID,NAME,GENDER,KOREAN_GRADE,ENGLISH_GRADE,MATH_GRADE
        sscanf(line, "%d,%49[^,],%4[^,],%d,%d,%d",
            &id, name, gender, &k, &e, &m);

        students[i].score1 = k;
        students[i].score2 = e;
        students[i].score3 = m;
        students[i].product = (long long)k * e * m;
    }
    fclose(fp);

    // 3. 난수 생성
    srand(time(NULL));
    long long target = rand() % 1000001;

    // 4. 순차 탐색
    long long seq_compare = sequential_search(students, N, target);

    // 5. 정렬 + 이진 탐색
    for (int i = 0; i < N; i++) sorted[i] = students[i];

    long long sort_compare = bubble_sort(sorted, N);
    long long binary_compare = binary_search(sorted, N, target);
    long long total_sorted = sort_compare + binary_compare;

    // 결과 출력
    printf("임의의 값: %lld\n", target);
    printf("순차 탐색 비교 횟수: %lld\n", seq_compare);
    printf("정렬 + 이진 탐색 비교 횟수: %lld\n", total_sorted);

    return 0;
}
