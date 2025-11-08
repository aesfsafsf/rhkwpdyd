#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define V 10       // 정점 수
#define E 20       // 간선 수
#define INF 999999 // 무한대

int main() {
    int graph[V][V];
    int i, j, e;

    srand((unsigned int)time(NULL));

    // 그래프 초기화
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
        }
    }

    // 무작위 간선 생성
    int count = 0;
    while (count < E) {
        int u = rand() % V;
        int v = rand() % V;
        int w = (rand() % 9) + 1; // 가중치 1~9

        if (u != v && graph[u][v] == INF) {
            graph[u][v] = w;
            graph[v][u] = w; // 무방향 그래프
            count++;
        }
    }

    // 생성된 그래프 출력
    printf("=== 인접 행렬 (가중치) ===\n");
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            if (graph[i][j] == INF)
                printf("INF\t");
            else
                printf("%d\t", graph[i][j]);
        }
        printf("\n");
    }

    // Floyd-Warshall 알고리즘
    int dist[V][V];
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    for (int k = 0; k < V; k++) {
        for (i = 0; i < V; i++) {
            for (j = 0; j < V; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    // 모든 노드 쌍 최단 거리 출력
    printf("\n=== 모든 노드 쌍의 최단 거리 ===\n");
    for (i = 0; i < V; i++) {
        for (j = i + 1; j < V; j++) {
            if (dist[i][j] == INF)
                printf("(%d, %d): INF\n", i, j);
            else
                printf("(%d, %d): %d\n", i, j, dist[i][j]);
        }
    }

    return 0;
}
