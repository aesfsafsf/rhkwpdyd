#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000
#define RUNS 100

/*------------------------------------------
  단순 삽입 정렬 (비교 횟수 측정)
------------------------------------------*/
long long insertion_sort(int arr[], int n)
{
    long long comps = 0;
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            comps++;  // 비교 발생
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            else break;
        }
        arr[j + 1] = key;
    }
    return comps;
}

/*------------------------------------------
  기본 Shell Sort (gap = n/2, n/4, ..., 1)
------------------------------------------*/
long long shell_sort_basic(int arr[], int n)
{
    long long comps = 0;

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= gap) {
                comps++;  // 비교
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                else break;
            }
            arr[j] = temp;
        }
    }
    return comps;
}

/*------------------------------------------
   최적화 Shell Sort (Knuth gap sequence)
------------------------------------------*/
long long shell_sort_optimal(int arr[], int n)
{
    long long comps = 0;

    int gap = 1;
    while (gap < n) gap = 3 * gap + 1;   // 증가시키기
    gap /= 3;                            // 가장 큰 gap부터 시작

    while (gap > 0) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= gap) {
                comps++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                else break;
            }
            arr[j] = temp;
        }
        gap /= 3;
    }

    return comps;
}

/*------------------------------------------
  배열 복사
------------------------------------------*/
void copy_array(int dest[], int src[])
{
    for (int i = 0; i < N; i++) dest[i] = src[i];
}

/*------------------------------------------
  메인 함수
------------------------------------------*/
int main()
{
    srand(time(NULL));

    long long ins_total = 0;
    long long shell_basic_total = 0;
    long long shell_opt_total = 0;

    int original[N];
    int temp[N];

    for (int r = 0; r < RUNS; r++) {

        // 랜덤 데이터 생성
        for (int i = 0; i < N; i++) {
            original[i] = rand() % 1000001;
        }

        // 단순 삽입 정렬
        copy_array(temp, original);
        ins_total += insertion_sort(temp, N);

        // 기본 쉘 정렬
        copy_array(temp, original);
        shell_basic_total += shell_sort_basic(temp, N);

        // 최적화 쉘 정렬 (Knuth gap)
        copy_array(temp, original);
        shell_opt_total += shell_sort_optimal(temp, N);
    }

    printf("\n===== 평균 비교 횟수 (100회 실행) =====\n");
    printf("단순 삽입 정렬 평균 비교 횟수         : %lld\n", ins_total / RUNS);
    printf("기본 Shell Sort 평균 비교 횟수        : %lld\n", shell_basic_total / RUNS);
    printf("최적화 Shell Sort (Knuth) 평균 비교 횟수 : %lld\n", shell_opt_total / RUNS);

    return 0;
}