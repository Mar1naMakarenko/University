#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

enum GraphType { DIRECTED, UNDIRECTED };

class GraphMatrix {
public:
    vector<vector<int>> adjMatrix;
    int numVertices;

    GraphMatrix(int vertices) : numVertices(vertices) {
        adjMatrix.resize(vertices, vector<int>(vertices, 0));
    }

    void addEdge(int u, int v, int weight = 1, GraphType type = UNDIRECTED) {
        adjMatrix[u][v] = weight;
        if (type == UNDIRECTED) {
            adjMatrix[v][u] = weight;
        }
    }

    void display() {
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

class GraphList {
public:
    vector<vector<pair<int, int>>> adjList;
    int numVertices;

    GraphList(int vertices) : numVertices(vertices) {
        adjList.resize(vertices);
    }

    void addEdge(int u, int v, int weight = 1, GraphType type = UNDIRECTED) {
        adjList[u].push_back(make_pair(v, weight));
        if (type == UNDIRECTED) {
            adjList[v].push_back(make_pair(u, weight));
        }
    }

    void display() {
        for (int i = 0; i < numVertices; i++) {
            cout << i << ": ";
            for (auto &neighbor : adjList[i]) {
                cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
            }
            cout << endl;
        }
    }
};

// Function to convert GraphMatrix to GraphList
GraphList convertMatrixToList(GraphMatrix &matrixGraph) {
    GraphList listGraph(matrixGraph.numVertices);
    for (int i = 0; i < matrixGraph.numVertices; i++) {
        for (int j = 0; j < matrixGraph.numVertices; j++) {
            if (matrixGraph.adjMatrix[i][j] != 0) {
                listGraph.addEdge(i, j, matrixGraph.adjMatrix[i][j], DIRECTED);
            }
        }
    }
    return listGraph;
}

// Function to convert GraphList to GraphMatrix
GraphMatrix convertListToMatrix(GraphList &listGraph) {
    GraphMatrix matrixGraph(listGraph.numVertices);
    for (int i = 0; i < listGraph.numVertices; i++) {
        for (auto &neighbor : listGraph.adjList[i]) {
            matrixGraph.addEdge(i, neighbor.first, neighbor.second, DIRECTED);
        }
    }
    return matrixGraph;
}

// Function to generate a random graph
GraphMatrix generateRandomGraphMatrix(int vertices, int edges, GraphType type = UNDIRECTED) {
    GraphMatrix graph(vertices);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, vertices - 1);
    uniform_int_distribution<> weight_dis(1, 10);

    for (int i = 0; i < edges; i++) {
        int u = dis(gen);
        int v = dis(gen);
        while (u == v) {
            v = dis(gen);
        }
        int weight = weight_dis(gen);
        graph.addEdge(u, v, weight, type);
    }
    return graph;
}

// Function to generate a random graph using adjacency list
GraphList generateRandomGraphList(int vertices, int edges, GraphType type = UNDIRECTED) {
    GraphList graph(vertices);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, vertices - 1);
    uniform_int_distribution<> weight_dis(1, 10);

    for (int i = 0; i < edges; i++) {
        int u = dis(gen);
        int v = dis(gen);
        while (u == v) {
            v = dis(gen);
        }
        int weight = weight_dis(gen);
        graph.addEdge(u, v, weight, type);
    }
    return graph;
}

// Depth-First Search (DFS)
void DFS(GraphMatrix &graph, int start, vector<bool> &visited) {
    stack<int> s;
    s.push(start);
    while (!s.empty()) {
        int vertex = s.top();
        s.pop();
        if (!visited[vertex]) {
            cout << vertex << " ";
            visited[vertex] = true;
        }
        for (int i = 0; i < graph.numVertices; i++) {
            if (graph.adjMatrix[vertex][i] != 0 && !visited[i]) {
                s.push(i);
            }
        }
    }
}

void DFS(GraphList &graph, int start, vector<bool> &visited) {
    stack<int> s;
    s.push(start);
    while (!s.empty()) {
        int vertex = s.top();
        s.pop();
        if (!visited[vertex]) {
            cout << vertex << " ";
            visited[vertex] = true;
        }
        for (auto &neighbor : graph.adjList[vertex]) {
            if (!visited[neighbor.first]) {
                s.push(neighbor.first);
            }
        }
    }
}

// Dijkstra's Algorithm
void dijkstra(GraphMatrix &graph, int src) {
    vector<int> dist(graph.numVertices, INT_MAX);
    dist[src] = 0;
    vector<bool> sptSet(graph.numVertices, false);

    for (int count = 0; count < graph.numVertices - 1; count++) {
        int u = -1;
        for (int i = 0; i < graph.numVertices; i++) {
            if (!sptSet[i] && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }
        sptSet[u] = true;
        for (int v = 0; v < graph.numVertices; v++) {
            if (!sptSet[v] && graph.adjMatrix[u][v] && dist[u] != INT_MAX && dist[u] + graph.adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph.adjMatrix[u][v];
            }
        }
    }
    for (int i = 0; i < graph.numVertices; i++) {
        cout << "Distance from " << src << " to " << i << " is " << dist[i] << endl;
    }
}

void dijkstra(GraphList &graph, int src) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(graph.numVertices, INT_MAX);
    pq.push(make_pair(0, src));
    dist[src] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        for (auto &neighbor : graph.adjList[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    for (int i = 0; i < graph.numVertices; i++) {
        cout << "Distance from " << src << " to " << i << " is " << dist[i] << endl;
    }
}

int main() {
    int mode;
    cout << "Select mode: 1-Interactive, 2-Demo, 3-Benchmark: ";
    cin >> mode;

    if (mode == 1) {
        // Interactive Mode
        int vertices, edges;
        cout << "Enter number of vertices: ";
        cin >> vertices;
        cout << "Enter number of edges: ";
        cin >> edges;

        GraphMatrix matrixGraph(vertices);
        GraphList listGraph(vertices);

        for (int i = 0; i < edges; i++) {
            int u, v, weight;
            cout << "Enter edge (u, v, weight): ";
            cin >> u >> v >> weight;
            matrixGraph.addEdge(u, v, weight);
            listGraph.addEdge(u, v, weight);
        }

        cout << "Matrix representation:" << endl;
        matrixGraph.display();
        cout << "List representation:" << endl;
        listGraph.display();

        // Example DFS
        vector<bool> visited(vertices, false);
        cout << "DFS from vertex 0 (Matrix): ";
        DFS(matrixGraph, 0, visited);
        cout << endl;

        visited.assign(vertices, false);
        cout << "DFS from vertex 0 (List): ";
        DFS(listGraph, 0, visited);
        cout << endl;

        // Example Dijkstra
        cout << "Dijkstra from vertex 0 (Matrix):" << endl;
        dijkstra(matrixGraph, 0);
        cout << "Dijkstra from vertex 0 (List):" << endl;
        dijkstra(listGraph, 0);
    } else if (mode == 2) {
        // Demo Mode
        int vertices = 5;
        int edges = 7;
        GraphMatrix matrixGraph = generateRandomGraphMatrix(vertices, edges);
        GraphList listGraph = convertMatrixToList(matrixGraph);

        cout << "Matrix representation:" << endl;
        matrixGraph.display();
        cout << "List representation:" << endl;
        listGraph.display();

        // Example DFS
        vector<bool> visited(vertices, false);
        cout << "DFS from vertex 0 (Matrix): ";
        DFS(matrixGraph, 0, visited);
        cout << endl;

        visited.assign(vertices, false);
        cout << "DFS from vertex 0 (List): ";
        DFS(listGraph, 0, visited);
        cout << endl;

        // Example Dijkstra
        cout << "Dijkstra from vertex 0 (Matrix):" << endl;
        dijkstra(matrixGraph, 0);
        cout << "Dijkstra from vertex 0 (List):" << endl;
        dijkstra(listGraph, 0);
    } else if (mode == 3) {
        // Benchmark Mode
        int vertices = 1000;
        int edges = 5000;

        auto start = chrono::high_resolution_clock::now();
        GraphMatrix matrixGraph = generateRandomGraphMatrix(vertices, edges);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end - start;
        cout << "Matrix generation time: " << diff.count() << " s\n";

        start = chrono::high_resolution_clock::now();
        GraphList listGraph = generateRandomGraphList(vertices, edges);
        end = chrono::high_resolution_clock::now();
        diff = end - start;
        cout << "List generation time: " << diff.count() << " s\n";

        // DFS Benchmark
        vector<bool> visited(vertices, false);
        start = chrono::high_resolution_clock::now();
        DFS(matrixGraph, 0, visited);
        end = chrono::high_resolution_clock::now();
        diff = end - start;
        cout << "DFS (Matrix) time: " << diff.count() << " s\n";

        visited.assign(vertices, false);
        start = chrono::high_resolution_clock::now();
        DFS(listGraph, 0, visited);
        end = chrono::high_resolution_clock::now();
        diff = end - start;
        cout << "DFS (List) time: " << diff.count() << " s\n";

        // Dijkstra Benchmark
        start = chrono::high_resolution_clock::now();
        dijkstra(matrixGraph, 0);
        end = chrono::high_resolution_clock::now();
        diff = end - start;
        cout << "Dijkstra (Matrix) time: " << diff.count() << " s\n";

        start = chrono::high_resolution_clock::now();
        dijkstra(listGraph, 0);
        end = chrono::high_resolution_clock::now();
        diff = end - start;
        cout << "Dijkstra (List) time: " << diff.count() << " s\n";
    }

    return 0;
}
