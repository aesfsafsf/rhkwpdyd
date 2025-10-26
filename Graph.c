#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define V 100
#define SPARSE_EDGES 100
#define DENSE_EDGES 4000

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node* head[V];
} AdjListGraph;

typedef struct {
    int matrix[V][V];
} AdjMatrixGraph;

// ==== 공통 함수 ====
int comparisons = 0;

int rand_vertex() {
    return rand() % V;
}

// 간선 중복 방지용 체크
int exists_edge_matrix(AdjMatrixGraph* g, int u, int v) {
    comparisons++;
    return g->matrix[u][v];
}

int exists_edge_list(AdjListGraph* g, int u, int v) {
    Node* temp = g->head[u];
    while (temp) {
        comparisons++;
        if (temp->vertex == v) return 1;
        temp = temp->next;
    }
    return 0;
}

// ==== 인접 행렬 방식 ====
AdjMatrixGraph* create_matrix_graph() {
    AdjMatrixGraph* g = malloc(sizeof(AdjMatrixGraph));
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            g->matrix[i][j] = 0;
    return g;
}

void add_edge_matrix(AdjMatrixGraph* g, int u, int v) {
    comparisons++;
    g->matrix[u][v] = g->matrix[v][u] = 1;
}

void remove_edge_matrix(AdjMatrixGraph* g, int u, int v) {
    comparisons++;
    g->matrix[u][v] = g->matrix[v][u] = 0;
}

void print_adjacent_matrix(AdjMatrixGraph* g, int u) {
    for (int v = 0; v < V; v++) {
        comparisons++;
        if (g->matrix[u][v])
            printf("%d ", v);
    }
    printf("\n");
}

// ==== 인접 리스트 방식 ====
AdjListGraph* create_list_graph() {
    AdjListGraph* g = malloc(sizeof(AdjListGraph));
    for (int i = 0; i < V; i++) g->head[i] = NULL;
    return g;
}

void add_edge_list(AdjListGraph* g, int u, int v) {
    Node* newNode = malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = g->head[u];
    g->head[u] = newNode;

    Node* newNode2 = malloc(sizeof(Node));
    newNode2->vertex = u;
    newNode2->next = g->head[v];
    g->head[v] = newNode2;
}

void remove_edge_list(AdjListGraph* g, int u, int v) {
    Node** cur = &g->head[u];
    while (*cur) {
        comparisons++;
        if ((*cur)->vertex == v) {
            Node* temp = *cur;
            *cur = (*cur)->next;
            free(temp);
            break;
        }
        cur = &((*cur)->next);
    }
}

void print_adjacent_list(AdjListGraph* g, int u) {
    Node* temp = g->head[u];
    while (temp) {
        comparisons++;
        printf("%d ", temp->vertex);
        temp = temp->next;
    }
    printf("\n");
}

// ==== 그래프 랜덤 생성 ====
void generate_random_edges_matrix(AdjMatrixGraph* g, int edges) {
    int count = 0;
    while (count < edges) {
        int u = rand_vertex(), v = rand_vertex();
        if (u != v && !exists_edge_matrix(g, u, v)) {
            add_edge_matrix(g, u, v);
            count++;
        }
    }
}

void generate_random_edges_list(AdjListGraph* g, int edges) {
    int count = 0;
    while (count < edges) {
        int u = rand_vertex(), v = rand_vertex();
        if (u != v && !exists_edge_list(g, u, v)) {
            add_edge_list(g, u, v);
            count++;
        }
    }
}

// ==== 메모리 사용량 계산 ====
size_t memory_usage_matrix() {
    return sizeof(AdjMatrixGraph);
}

size_t memory_usage_list(AdjListGraph* g) {
    size_t size = sizeof(AdjListGraph);
    for (int i = 0; i < V; i++) {
        Node* temp = g->head[i];
        while (temp) {
            size += sizeof(Node);
            temp = temp->next;
        }
    }
    return size;
}

// ==== 메인 함수 ====
int main() {
    srand(time(NULL));

    // 1. 희소 그래프 - 인접 행렬
    comparisons = 0;
    AdjMatrixGraph* sparseM = create_matrix_graph();
    generate_random_edges_matrix(sparseM, SPARSE_EDGES);
    printf("[희소-행렬] 메모리: %zu bytes, 비교: %d\n",
        memory_usage_matrix(), comparisons);

    // 2. 희소 그래프 - 인접 리스트
    comparisons = 0;
    AdjListGraph* sparseL = create_list_graph();
    generate_random_edges_list(sparseL, SPARSE_EDGES);
    printf("[희소-리스트] 메모리: %zu bytes, 비교: %d\n",
        memory_usage_list(sparseL), comparisons);

    // 3. 밀집 그래프 - 인접 행렬
    comparisons = 0;
    AdjMatrixGraph* denseM = create_matrix_graph();
    generate_random_edges_matrix(denseM, DENSE_EDGES);
    printf("[밀집-행렬] 메모리: %zu bytes, 비교: %d\n",
        memory_usage_matrix(), comparisons);

    // 4. 밀집 그래프 - 인접 리스트
    comparisons = 0;
    AdjListGraph* denseL = create_list_graph();
    generate_random_edges_list(denseL, DENSE_EDGES);
    printf("[밀집-리스트] 메모리: %zu bytes, 비교: %d\n",
        memory_usage_list(denseL), comparisons);

    // 예시로 연결 확인 및 인접 노드 출력 테스트
    comparisons = 0;
    int u = 0, v = 1;
    exists_edge_matrix(sparseM, u, v);
    printf("[희소-행렬 연결확인 비교횟수] %d\n", comparisons);

    comparisons = 0;
    exists_edge_list(sparseL, u, v);
    printf("[희소-리스트 연결확인 비교횟수] %d\n", comparisons);

    comparisons = 0;
    print_adjacent_matrix(sparseM, u);
    printf("[희소-행렬 인접노드 출력 비교횟수] %d\n", comparisons);

    comparisons = 0;
    print_adjacent_list(sparseL, u);
    printf("[희소-리스트 인접노드 출력 비교횟수] %d\n", comparisons);

    return 0;
}
